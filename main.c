/**
 *
 *   Authors: 
 *    Joao Bernardo Almeida Santos, n. 84083
 *    Simão Gonçalves Eusébio, n. 84183
 *
 *   MEEC - Instituto Superior Técnico
 *
 *
 *   Graphical Interface provided by Professor João Ascenso
 *
 *   Blackjack Game
 *
 *
 */
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

#define MIN_ARGUMENTS 3 // minimum number of main arguments
#define AI_DELAY_UNIT 250 // ai delay unit used for increasing and decreasing ai delay

int main(int argc, char **argv) {
    SDL_Window* window = NULL; // game window
    SDL_Renderer* renderer = NULL; // sdl renderer used for rendering the graphical interface
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2]; // surfaces for the cards and images
    SDL_Event event; // current event
    TTF_Font *serif = NULL;
    int delay = 300; // interface refresh delay
    int aiDelay = 1000; // artificial inteligence delay
    int quit = 0; // quit variable
    Card_node *deck = NULL; // set of decks
    int numberOfDecks = 0; // number of decks used in the game
    bool turn_ended = false; // true value if turn has ended
    bool adding_player = false; // true value if 'a' option is selected
    Player_node *players = NULL; // active players list
    Player_node *removedPlayers = NULL; // non-active players list
    Player_node *currentPlayerNode = NULL; // current player
    Player *house = (Player *) malloc(sizeof(Player)); // house
    int newPlayerPos = -1; // player position for 'a' option use
    Player_node *player_node_aux = NULL; // auxiliary variable used to walk players list
    AIAction **ai_actions = NULL; // 2d matrix with ai actions
    char *gameConfig = NULL; // game configuration file name
    char *aiConfig = NULL; // ai configuration file name
    int hilo = 0; // hilo current value
    int cardsDealt = 0; // number of cards dealt
    int mouseX = 0, mouseY = 0; // mouse coordinates
    
    /* Checks if all needed arguments are specified */
    if (argc < MIN_ARGUMENTS) {
        printf("Usage: blackjack <game settings file> <ai settings file>\n");
        return EXIT_FAILURE;
    }
    
    gameConfig = argv[1]; // sets game configuration file name
    aiConfig = argv[2]; // sets ai configuration file name
    
    /* Memory allocation test */
    if (house == NULL) {
        ALLOCATION_ERROR_MESSAGE();
        return EXIT_FAILURE;
    }
    
    house->cards = NULL;
    house->money = 0;
    
    srand((unsigned int) time(NULL)); // initializes the random number generator
    
    /* Prints welcome message */
    printf("**************************\n*                        *\n*  Welcome to BlackJack  *\n*                        *\n**************************\n\n");
    
    /* Sets the game settings */
    GameSettings(gameConfig, aiConfig, &numberOfDecks, &players, &ai_actions);
    
    /* initialize graphics */
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    
    /* loads the cards images */
    LoadCards(cards);
    
    deck = DeckMaker(numberOfDecks); // loads a new deck of cards
    
    ShuffleCards(&deck, numberOfDecks);
    
    /* Begins a new turn */
    NewTurn(&deck, numberOfDecks, &players, &removedPlayers, &currentPlayerNode, house, &hilo, &cardsDealt);
    
    /* Main loop. Window remains open until quit == 1 */
    while( quit == 0 )
    {
        /* Checks if all players have played, if so ends turn. */
        if (!turn_ended && currentPlayerNode == NULL) {
            /**
             * Finishes the turn. The house plays and the money is updated.
             * After this function, it's possible to press the 'n' key
             * to begin a new turn.
             */
            FinishTurn(&deck, numberOfDecks, house, &players, &hilo, &cardsDealt);
            
            turn_ended = true;
        }
        
        /* If current player has a blackjack, max points or a bust stands! */
        if (currentPlayerNode != NULL && !currentPlayerNode->player.ai && currentPlayerNode->player.score >= BLACKJACK_MAX_SCORE) {
            currentPlayerNode = Stand(currentPlayerNode);
        }
        
        /* while there's events to handle */
        while( SDL_PollEvent( &event ) )
        {
            /* Handles events */
            if( event.type == SDL_QUIT )
            {
                quit = 1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && adding_player) {
                /**
                  * Checks if left button was pressed and 'a' option is selected, 
                  * if so gets the position that was clicked and adds a new player
                  * if possible.
                  *
                  */
                if (event.button.button == SDL_BUTTON_LEFT) {
                    newPlayerPos = GetPlayerPositionFromXY(event.button.x, event.button.y);
                    
                    if (newPlayerPos > 0 && newPlayerPos < 4) {
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
                        aiDelay += AI_DELAY_UNIT;
                        break;
                    case SDLK_DOWN:
                        if (aiDelay >= AI_DELAY_UNIT) {
                            aiDelay -= AI_DELAY_UNIT;
                        }
                        break;
                    case SDLK_s:
                        // stand !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            currentPlayerNode = Stand(currentPlayerNode);
                        }
                        
                        break;
                    case SDLK_h:
                        // hit !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            /* The current player requests a card. */
                            currentPlayerNode = Hit(&deck, currentPlayerNode, numberOfDecks, &hilo, &cardsDealt);
                        }
                        
                        break;
                    case SDLK_d:
                        // double !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            currentPlayerNode = Double(&deck, numberOfDecks, house, currentPlayerNode, &hilo, &cardsDealt);
                        }
                        break;
                    case SDLK_r:
                        // surrender !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            currentPlayerNode = Surrender(currentPlayerNode);
                        }
                        break;
                    case SDLK_b:
                        // bet !
                        /* Verifies the turn has ended */
                        if (turn_ended) {
                            DisplayBetMessage(renderer, serif);
                            SDL_RenderPresent(renderer);
                            
                            Bet(players);
                        }
                        break;
                    case SDLK_a:
                        // add !
                        /* Verifies the turn has ended */
                        if (turn_ended) {
                            adding_player = !adding_player;
                        }
                        break;
                    case SDLK_n:
                        /* Verifies if the turn has ended. If so, begins a new turn. */
                        if (turn_ended) {
                            NewTurn(&deck, numberOfDecks, &players, &removedPlayers, &currentPlayerNode, house, &hilo, &cardsDealt);
                            
                            turn_ended = (currentPlayerNode == NULL); // If there are no more player there is no new turn
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        
        /* render game table */
        RenderTable(players, currentPlayerNode, house, serif, imgs, renderer);
        /* render house cards */
        RenderHouseCards(house, house->cards, house->hand_size, cards, renderer);
        /* render player cards */
        RenderPlayerCards(players, cards, renderer);
        
        /* If 'a' option is selected, displays the player area hovered by the mouse */
        if (adding_player) {
            DisplayAddPlayerMessage(renderer, serif);
            
            SDL_GetMouseState(&mouseX, &mouseY); // gets mouse position
            DisplayAddPlayerHover(mouseX, mouseY, players, renderer); // renders it
        }
        
        /* render in the screen all changes above */
        SDL_RenderPresent(renderer);
        /* add a delay */
        SDL_Delay( delay );
        
        /* If current player is ai, plays it! */
        if (currentPlayerNode != NULL && currentPlayerNode->player.ai) {
            SDL_Delay(aiDelay); // waits ai delay
            
            PlayAI(&currentPlayerNode, house, ai_actions, &deck, numberOfDecks, &hilo, &cardsDealt); // plays !
            
            while( SDL_PollEvent( &event ) ) {
                if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)) {
                    quit = 1;
                }
            }
        }
    }
    
    /* free memory allocated for images and textures and close everything */
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    /* Checks if the house has cards */
    if (!empty(house->cards)) {
        erase_card_list(house->cards); // frees house cards
    }
    
    
    /* Frees active players cards, and adds all of them to removed players */
    while (players != NULL) {
        player_node_aux = players->next;
        
        players->next = NULL;
        erase_card_list(players->player.cards);
        join_player_node(&removedPlayers, players, 0);
        
        players = player_node_aux;
    }
    
    /* Writes stats file */
    WriteMoneyAndStatsToFile(removedPlayers, house, turn_ended);
    
    free(house);
    
    /* Frees players, deck and ai actions */
    erase_player_list(removedPlayers);
    EraseDeck(deck);
    FreeAIActions(ai_actions);
    
    return EXIT_SUCCESS;
}
