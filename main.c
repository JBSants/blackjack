#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "graphics.h"
#include "cards.h"
#include "deck.h"
#include "blackjack.h"

const char * playerNames[] = {"Player 1", "Player 2", "Player 3", "Player 4"};

void ReadGameParameters(int *numberOfDecks, int *initialMoney, int *betAmount)
{
    short notValid = 1; // used for parameter value check
    char buffer[255] = { 0 }; // buffer for fgets

    /* While the parameter entered is not valid prompt for it */
    while (notValid) {
        /* Prompts the parameter */
        printf("Please enter the number of decks (%d to %d) you wan't to use in your game: ", 1, 6);
        fgets(buffer, 255, stdin);
        sscanf(buffer, "%d", numberOfDecks);

        notValid = *numberOfDecks < 1 || *numberOfDecks > 6;

        /* If not valid show error */
        if (notValid) {
            printf("\n** Sorry, you have to select between %d to %d decks. **\n\n", 1, 6);
        }
    }

    notValid = 1; // resets not valid

    /* While the parameter entered is not valid prompt for it */
    while (notValid) {
        /* Prompts the parameter */
        printf("Please enter the initial amount of money for each player (> %d): ", 10);
        fgets(buffer, 255, stdin);
        sscanf(buffer, "%d", initialMoney);

        notValid = *initialMoney < 10;

        /* If not valid show error */
        if (notValid) {
            printf("\n** Sorry, each player has to have at least 10 EUR. **\n\n");
        }
    }

    notValid = 1; // resets not valid

    /* While the parameter entered is not valid prompt for it */
    while (notValid) {
        /* Determines maximum bet value */
        int betMaximumValue = 0.2 * (*initialMoney);

        /* Prompts the parameter */
        printf("Please enter the bet amount (%d to %d): ", 1, betMaximumValue);
        fgets(buffer, 255, stdin);
        sscanf(buffer, "%d", betAmount);

        notValid = *betAmount < 1 || *betAmount > betMaximumValue;

        /* If not valid show error */
        if (notValid) {
            printf("\n** Sorry, bet amount has to be from %d to %d. **\n\n", 1, betMaximumValue);
        }
    }
}

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
    int initialMoney = 0; // initial money
    int betAmount = 0; // bet amount
    int i = 0; // iterator
    bool turn_ended = 0; // true value if turn has ended
    Player_node *players = NULL; // players list
    Player_node *currentPlayerNode = NULL;
    Player *house = (Player *) malloc(sizeof(Player));

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

    /* Reads parameters */
    ReadGameParameters(&numberOfDecks, &initialMoney, &betAmount);

    for (i = 0; i < 4; i++) {
        Player *currentPlayer = (Player *) malloc(sizeof(Player));

        strcpy(currentPlayer->name, playerNames[i]);
        currentPlayer->money = (float) initialMoney;
        currentPlayer->bet = (float) betAmount;
        currentPlayer->cards = NULL;
        currentPlayer->hand_size = 0;
        currentPlayer->games_result.won = 0;
        currentPlayer->games_result.tied = 0;
        currentPlayer->games_result.lost = 0;
        currentPlayer->ai = false;
        currentPlayer->surrender = false;
        currentPlayer->score = 0;

        add_player(&players, *currentPlayer);
        
        free(currentPlayer);
    }

    // initialize graphics
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);

    deck = DeckMaker(numberOfDecks);

    ShuffleCards(&deck, numberOfDecks, 1); // TODO: Times really needed??

    DealCards(&deck, players, house, numberOfDecks, BLACKJACK_INITIAL_CARDS);

    currentPlayerNode = players;
    currentPlayerNode->player.money -= currentPlayerNode->player.bet;

    /* Main loop. Window remains open until quit == 1 */
    while( quit == 0 )
    {
      // while there's events to handle
      while( SDL_PollEvent( &event ) )
      {
        if( event.type == SDL_QUIT )
        {
          quit = 1;
        }
        else if ( event.type == SDL_KEYDOWN )
        {
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
                       FinishTurn(&deck, numberOfDecks, house, players);

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
                      FinishTurn(&deck, numberOfDecks, house, players);

                      turn_ended = true;
                  }
              }

              break;
            case SDLK_d:
                  if (!turn_ended) {
                      currentPlayerNode = Double(&deck, numberOfDecks, house, currentPlayerNode);
                      
                      if (currentPlayerNode == NULL) {
                          FinishTurn(&deck, numberOfDecks, house, players);
                          
                          turn_ended = true;
                      }
                  }
                  break;
            case SDLK_r:
                  if (!turn_ended) {
                      currentPlayerNode = Surrender(currentPlayerNode);
                      
                      if (currentPlayerNode == NULL) {
                          FinishTurn(&deck, numberOfDecks, house, players);
                          
                          turn_ended = true;
                      }
                  }
                  break;
            case SDLK_b:
                  if (turn_ended) {
                      Bet(players);
                  }
                  break;
            case SDLK_n:
              /* Verifies if the turn has ended. If so, begins a new turn. */
              if (turn_ended) {
                Player_node *player_node_aux = players;
                Card_node **house_cards = &(house->cards);

                while (player_node_aux != NULL) {
                    Card_node **player_cards = &(player_node_aux->player.cards);

                    while (*player_cards != NULL) {
                        free(pop_card(player_cards));
                    }

                    player_node_aux->player.score = 0;
                    player_node_aux->player.hand_size = 0;

                    player_node_aux = player_node_aux->next;
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
                  currentPlayerNode->player.money -= currentPlayerNode->player.bet;

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
    EraseDeck(deck);

    return EXIT_SUCCESS;
}
