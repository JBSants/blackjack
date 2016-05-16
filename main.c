#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "graphics.h"
#include "cards.h"
#include "deck.h"
#include "blackjack.h"
#include "config.h"

int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    TTF_Font *serif = NULL;
    int delay = 300; // interface refresh delay
    int quit = 0; // quit variable
    Card_node *deck = NULL; // set of decks
    int numberOfDecks = 0; // number of decks used in the game
    bool turn_ended = false; // true value if turn has ended
    bool adding_player = false;
    Player_node *players = NULL; // players list
    Player_node *removedPlayers = NULL;
    Player_node *currentPlayerNode = NULL;
    Player *house = (Player *) malloc(sizeof(Player));
    int i = 0, newPlayerPos = -1;
    Player_node *player_node_aux = NULL;
    Card_node **house_cards = &(house->cards);
    Player_node *tmp_player = NULL;
    
    if (house == NULL) {
        ERROR_MESSAGE();
        return EXIT_FAILURE;
    }
    
    house->ai = true;
    house->score = 0;
    house->hand_size = 0;
    house->cards = NULL;
    
    srand((unsigned int) time(NULL)); // initializes the random number generator
    
    /* Prints welcome message */
    printf("**************************\n*                        *\n*  Welcome to BlackJack  *\n*                        *\n**************************\n\n");
    
    GameSettings("config1.txt", "config2.txt", &numberOfDecks, &players);
    
    // initialize graphics
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);
    
    deck = DeckMaker(numberOfDecks);
    
    ShuffleCards(&deck, numberOfDecks, 1); // TODO: Times really needed??
    
    DealCards(&deck, players, house, numberOfDecks, BLACKJACK_INITIAL_CARDS);
    
    currentPlayerNode = players;
    
    player_node_aux = currentPlayerNode;
    
    while (player_node_aux != NULL) {
        player_node_aux->player.money -= player_node_aux->player.bet;
        
        player_node_aux = player_node_aux->next;
    }
    
    /* Main loop. Window remains open until quit == 1 */
    while( quit == 0 )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit = 1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && adding_player) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    newPlayerPos = GetPlayerPositionFromXY(event.button.x, event.button.y);
                    
                    if (newPlayerPos != -1) {
                        AddNewPlayer(&players, newPlayerPos);
                    }
                    
                    adding_player = false;
                }
            } else if ( event.type == SDL_KEYDOWN ) {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_q:
                        quit = 1; // Quits
                        break;
                    case SDLK_s:
                        // stand !
                        /* Verifies the turn hasn't end */
                        if (!turn_ended) {
                            currentPlayerNode = Stand(currentPlayerNode);
                            
                            /* If there are no more players after this stand */
                            if (currentPlayerNode == NULL) {
                                /**
                                 * Finishes the turn. The house plays and the money is updated.
                                 * After this function, it's possible to press the 'n' key
                                 * to begin a new turn.
                                 */
                                FinishTurn(&deck, numberOfDecks, house, &players);
                                
                                turn_ended = true;
                            }
                        }
                        
                        break;
                    case SDLK_h:
                        // hit !
                        /* Verifies the turn hasn't end */
                        if (!turn_ended) {
                            /* The current player requests a card. */
                            currentPlayerNode = Hit(&deck, currentPlayerNode, numberOfDecks);
                            
                            /* If there are no more players after this stand */
                            if (currentPlayerNode == NULL) {
                                FinishTurn(&deck, numberOfDecks, house, &players);
                                
                                turn_ended = true;
                            }
                        }
                        
                        break;
                    case SDLK_d:
                        if (!turn_ended) {
                            currentPlayerNode = Double(&deck, numberOfDecks, house, currentPlayerNode);
                            
                            if (currentPlayerNode == NULL) {
                                FinishTurn(&deck, numberOfDecks, house, &players);
                                
                                turn_ended = true;
                            }
                        }
                        break;
                    case SDLK_r:
                        if (!turn_ended) {
                            currentPlayerNode = Surrender(currentPlayerNode);
                            
                            if (currentPlayerNode == NULL) {
                                FinishTurn(&deck, numberOfDecks, house, &players);
                                
                                turn_ended = true;
                            }
                        }
                        break;
                    case SDLK_b:
                        if (turn_ended) {
                            Bet(players);
                        }
                        break;
                    case SDLK_a:
                        if (turn_ended) {
                            adding_player = true;
                        }
                        break;
                    case SDLK_n:
                        /* Verifies if the turn has ended. If so, begins a new turn. */
                        if (turn_ended) {
                            player_node_aux = players;
                            tmp_player = NULL;
                            
                            i = 0;
                            
                            while (player_node_aux != NULL) {
                                Card_node **player_cards = &(player_node_aux->player.cards);
                                
                                while (*player_cards != NULL) {
                                    free(pop_card(player_cards));
                                }
                                
                                player_node_aux->player.score = 0;
                                player_node_aux->player.hand_size = 0;
                                player_node_aux->player.money -= player_node_aux->player.bet;
                                
                                if (player_node_aux->player.money <= 0 && player_node_aux->player.bet <= 0) {
                                    tmp_player = player_node_aux->next;
                                    join_player_node(&removedPlayers, take_player_node(&players, i), 0);
                                    player_node_aux = tmp_player;
                                } else {
                                    player_node_aux = player_node_aux->next;
                                }
                                
                                i += 1;
                            }
                            
                            while (*house_cards != NULL) {
                                free(pop_card(house_cards));
                            }
                            
                            house->score = 0;
                            house->hand_size = 0;
                            
                            /* Verfies if there are any dealt cards. If not, quits the game
                             * diplaying a message.
                             */
                            if (!DealCards(&deck, players, house, numberOfDecks, BLACKJACK_INITIAL_CARDS)) {
                                printf("No more players! Thank you for playing Blackjack!\n");
                                quit = 1;
                                break;
                            }
                            
                            /* Stands until the current player hasn't a blackjack */
                            currentPlayerNode = players;
                            
                            turn_ended = false;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        
        
        // render game table
        RenderTable(players, currentPlayerNode, house, serif, imgs, renderer);
        // render house cards
        RenderHouseCards(house, house->cards, house->hand_size, cards, renderer);
        // render player cards
        RenderPlayerCards(players, cards, renderer);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );
    }
    
    // free memory allocated for images and textures and close everything
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    /* Writes stats file */
    //WriteMoneyAndStatsToFile(money, player_stats);
    
    free(house);
    erase_player_list(players);
    erase_player_list(removedPlayers);
    EraseDeck(deck);
    
    return EXIT_SUCCESS;
}
