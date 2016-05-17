#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "graphics.h"
#include "cards.h"
#include "deck.h"
#include "blackjack.h"
#include "config.h"
#include "ai.h"

#define MIN_ARGUMENTS 3

int main(int argc, char **argv) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    TTF_Font *serif = NULL;
    int delay = 300; // interface refresh delay
    int aiDelay = 1000;
    int quit = 0; // quit variable
    Card_node *deck = NULL; // set of decks
    int numberOfDecks = 0; // number of decks used in the game
    bool turn_ended = false; // true value if turn has ended
    bool adding_player = false;
    Player_node *players = NULL; // players list
    Player_node *removedPlayers = NULL;
    Player_node *currentPlayerNode = NULL;
    Player *house = (Player *) malloc(sizeof(Player));
    int newPlayerPos = -1;
    Player_node *player_node_aux = NULL;
    AIAction **ai_actions = NULL;
    char *gameConfig = NULL;
    char *aiConfig = NULL;
    
    if (argc < MIN_ARGUMENTS) {
        printf("Usage: blackjack <game settings file> <ai settings file>\n");
        return EXIT_FAILURE;
    }
    
    gameConfig = argv[1];
    aiConfig = argv[2];
    
    if (house == NULL) {
        ERROR_MESSAGE();
        return EXIT_FAILURE;
    }
    
    house->ai = true;
    house->cards = NULL;
    
    srand((unsigned int) time(NULL)); // initializes the random number generator
    
    /* Prints welcome message */
    printf("**************************\n*                        *\n*  Welcome to BlackJack  *\n*                        *\n**************************\n\n");
    
    GameSettings(gameConfig, aiConfig, &numberOfDecks, &players, &ai_actions);
    
    // initialize graphics
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);
    
    deck = DeckMaker(numberOfDecks);
    
    ShuffleCards(&deck, numberOfDecks);
    
    NewTurn(&deck, numberOfDecks, &players, &removedPlayers, &currentPlayerNode, house);
    
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
                    case SDLK_UP:
                        aiDelay += 250;
                        break;
                    case SDLK_DOWN:
                        if (aiDelay > 0) {
                            aiDelay -= 250;
                        }
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
                            NewTurn(&deck, numberOfDecks, &players, &removedPlayers, &currentPlayerNode, house);
                            
                            turn_ended = (currentPlayerNode == NULL);
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
        
        if (currentPlayerNode != NULL && currentPlayerNode->player.ai) {
            SDL_Delay(aiDelay);
            
            PlayAI(&currentPlayerNode, house, ai_actions, &deck, numberOfDecks);
            
            if (currentPlayerNode == NULL) {
                FinishTurn(&deck, numberOfDecks, house, &players);
                
                turn_ended = true;
            }
            
            while( SDL_PollEvent( &event ) ) {
                if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)) {
                    quit = 1;
                }
            }
        }
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
    
    while (players != NULL) {
        player_node_aux = players->next;
        
        players->next = NULL;
        join_player_node(&removedPlayers, players, 0);
        
        players = player_node_aux;
    }
    
    erase_player_list(removedPlayers);
    EraseDeck(deck);
    FreeAIActions(ai_actions);
    
    return EXIT_SUCCESS;
}
