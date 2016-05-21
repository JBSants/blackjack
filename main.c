/**************************************
* PROGRAM NAME: Blackjack Game
* AUTHORS: João Santos e Simão Eusébio
*   |IST INFO|: Joao Bernardo Almeida Santos, n. 84083
*               Simão Gonçalves Eusébio, n. 84183
*
*               MEEC - Instituto Superior Técnico
* 
* DESCRIPTION: This program simulates the game Blackjack also known as twenty-one, which is the most widely played casino banking game in the world.
* Blackjack is a comparing card game between player and dealer, meaning players compete against the dealer but not against other players.
* FEATURES: Players' scores are shown in a graphical window for a more comfort play along with the money and cards of each player and the cards of the house.
* This implementation of the game only as the following game moves: hit, stand, double and surrender. Each one with a keyboard key associated with it ('h', 's', 'd' and 'r' respectively), and there is also four more user available keys,
* 'n' to start the next round, 'q' to quit the game, 'b' to change the bet of a player and 'a' to add a new player to the game (if possible).
* Before quitting the program, a game statistics file is written with the overall game results of each player and the house's money losses/gains.
* Also, when occurs a bust, the 'busted' player or house are marked  in the graphical interface (with a 'bust').
* The house routine is inside a finish turn routine which performs all the actions that basically finish that round including the the house play that consists
* in performing a stand if the house score is greater than 16.
* 
* PARAMETERS: Concerning to the game parameters, the program takes two files as program's arguments: the congiguration file and the AI strategy file.
*
*  NOTES: Every function has a small description, parameter informations, when assumed relevant, and return values information, also when assumed relevant on the top of its definition. Besides they have some helpfull
*  commentaries inside the definition. Commentaries are also made when assumed relevant in other blocks of code along the main program.
* DATE: 21/5/2016

* CREDITS: Graphical Interface provided by Professor João Ascenso ; goto AUTHORS
************************************************************************************************************************************************************************************************************************/

#include <stdio.h>
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

#define MIN_ARGUMENTS 3 // Minimum number of main arguments
#define AI_DELAY_UNIT 250 // AI delay unit used for increasing and decreasing ai delay

int main(int argc, char **argv) {
    SDL_Window* window = NULL; // Game window
    SDL_Renderer* renderer = NULL; // SDL renderer used for rendering the graphical interface
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2]; // Surfaces for the cards and images
    SDL_Event event; // Current event
    TTF_Font *serif = NULL;
    int delay = 300; // Interface refresh delay
    int aiDelay = 1000; // Artificial inteligence delay
    int quit = 0; // Quit variable
    Card_node *deck = NULL; // Set of decks
    int numberOfDecks = 0; // Number of decks used in the game
    bool turn_ended = false; // True value if turn has ended
    bool adding_player = false; // True value if 'a' option is selected
    Player_node *players = NULL; // Active players list
    Player_node *removedPlayers = NULL; // Non-active players list
    Player_node *currentPlayerNode = NULL; // Current player
    Player *house = (Player *) malloc(sizeof(Player)); // House
    int newPlayerPos = -1; // Player position for 'a' option use
    Player_node *player_node_aux = NULL; // Auxiliary variable used to walk players list
    AIAction **ai_actions = NULL; // 2d matrix with ai actions
    char *gameConfig = NULL; // Game configuration file name
    char *aiConfig = NULL; // AI configuration file name
    int hilo = 0; // Hilo current value
    int cardsDealt = 0; // Number of cards dealt
    int mouseX = 0, mouseY = 0; // Mouse coordinates
    
    /* Checks if all needed arguments are specified */
    if (argc < MIN_ARGUMENTS) {
        printf("Usage: blackjack <game settings file> <ai settings file>\n");
        return EXIT_FAILURE;
    }
    
    gameConfig = argv[1]; // Sets game configuration file name
    aiConfig = argv[2]; // Sets ai configuration file name
    
    /* Memory allocation test */
    if (house == NULL) {
        ALLOCATION_ERROR_MESSAGE();
        return EXIT_FAILURE;
    }
    
    house->cards = NULL;
    house->money = 0;
    
    srand((unsigned int) time(NULL)); // Initializes the random number generator
    
    /* Prints welcome message */
    printf("**************************\n*                        *\n*  Welcome to BlackJack  *\n*                        *\n**************************\n\n");
    
    /* Sets the game settings */
    GameSettings(gameConfig, aiConfig, &numberOfDecks, &players, &ai_actions);
    
    /* initialize graphics */
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    
    /* Loads the cards images */
    LoadCards(cards);
    
    deck = DeckMaker(numberOfDecks); // Loads a new deck of cards
    
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
        
        /* While there's events to handle */
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
                    
                    if (newPlayerPos >= (MIN_PLAYERS - 1) && newPlayerPos < MAX_PLAYERS) {
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
                        // Stand !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            currentPlayerNode = Stand(currentPlayerNode);
                        }
                        
                        break;
                    case SDLK_h:
                        // Hit !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            /* The current player requests a card. */
                            currentPlayerNode = Hit(&deck, currentPlayerNode, numberOfDecks, &hilo, &cardsDealt);
                        }
                        
                        break;
                    case SDLK_d:
                        // Double !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            currentPlayerNode = Double(&deck, numberOfDecks, house, currentPlayerNode, &hilo, &cardsDealt);
                        }
                        break;
                    case SDLK_r:
                        // Surrender !
                        /* Verifies the turn hasn't ended */
                        if (!turn_ended) {
                            currentPlayerNode = Surrender(currentPlayerNode);
                        }
                        break;
                    case SDLK_b:
                        // Bet !
                        /* Verifies the turn has ended */
                        if (turn_ended) {
                            DisplayBetMessage(renderer, serif);
                            SDL_RenderPresent(renderer);
                            
                            Bet(players);
                        }
                        break;
                    case SDLK_a:
                        // Add !
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
        
        /* Render game table */
        RenderTable(players, currentPlayerNode, house, serif, imgs, renderer);
        /* Render house cards */
        RenderHouseCards(house, house->cards, house->hand_size, cards, renderer);
        /* Render player cards */
        RenderPlayerCards(players, cards, renderer);
        
        /* If 'a' option is selected, displays the player area hovered by the mouse */
        if (adding_player) {
            DisplayAddPlayerMessage(renderer, serif);
            
            SDL_GetMouseState(&mouseX, &mouseY); // gets mouse position
            DisplayAddPlayerHover(mouseX, mouseY, players, renderer); // renders it
        }
        
        /* Render in the screen all changes above */
        SDL_RenderPresent(renderer);
        /* Add a delay */
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
