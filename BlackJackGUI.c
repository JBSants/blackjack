/**
  *
  *   Autor: João Bernardo Almeida Santos <joao.almeida.santos@tecnico.ulisboa.pt>
  *   Número: 84083
  *   Curso: MEEC - Instituto Superior Técnico
  *
  *
  *   Interface gráfica disponibilizada pelo Professor João Ascenso
  *
  *   Jogo de Blackjack
  *
  *
  */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define STRING_SIZE 100       // max size for some strings
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define MAX_DECK_SIZE 52      // number of max cards in the deck
#define MAX_NUMBER_OF_DECKS 6   // maximum number of decks to play
#define MIN_NUMBER_OF_DECKS 1
#define MAX_CARD_HAND 11      // 11 cards max. that each player can hold
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4         // number of maximum players
#define MIN_INITIAL_MONEY 10  // minimum amount of initial money for each player
#define MIN_BET_VALUE 1 // minimum bet amount
#define MAX_BET_VALUE_PERCENTAGE 0.2 // maximum bet amount (% of initial money)
#define INITIAL_CARDS_AMOUNT 2 // number of cards to be initially dealt
#define ACE_ID 12 // card id of ace
#define ACE_VALUE_MAX 11 // maximum points of ace card
#define ACE_VALUE_MIN 1 // minimum points of ace card
#define MAXIMUM_POINTS 21 // maximum points
#define FIGURE_VALUE 10 // points of figure cards

// declaration of the functions related to graphical issues
void InitEverything(int , int , SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(int [], SDL_Surface **, SDL_Renderer *, int);
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(int [], int , SDL_Surface **, SDL_Renderer * );
void RenderPlayerCards(int [][MAX_CARD_HAND], int [], SDL_Surface **, SDL_Renderer * );
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);
void Hit(int *deck, int numberOfCards, int *currentCard, int *cards, int *pos_hand, int *points);
void Stand(int *currentPlayer, int bet, int *money, int player_cards[][MAX_CARD_HAND], int *pos_player_hand, int *player_points);
void ReadGameParameters(int *, int *, int *);
int InitializeDeck(int *, int);
void ShuffleDeck(int *, int);
void Swap(int *, int *);
short DealCards(int *, int *, int [][MAX_CARD_HAND], int *, int *, int *, int *, int);
int *NextCard(int *, int *);
void DeterminePoints(int *player_points, int *cards, int pos_player_hand);
short IsAce(int);
int PointsFromCardID(int id);
short Bust(int);
int PlayHouse(int *house_cards, int *pos_house_hand, int *, int, int *);
void UpdateMoney(int *money, int bet, int *player_points, int house_points);
void FinishTurn(int *deck, int numberOfCards, int *currentCard, int *money, int betAmount, int *house_cards, int *pos_house_hand, int *player_points);


// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Joao Almeida Santos";
const char myNumber[] = "84083";
const char * playerNames[] = {"Player 1", "Player 2", "Player 3", "Player 4"};

/**
* main function: entry point of the program
* only to invoke other functions !
*/
int main( int argc, char* args[] )
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
  SDL_Event event;
  int delay = 300;
  int quit = 0;
  int money[MAX_PLAYERS] = { 0 };
  int player_cards[MAX_PLAYERS][MAX_CARD_HAND] = {{ 0 }};
  int player_points[MAX_PLAYERS] = { 0 };
  int house_cards[MAX_CARD_HAND] = { 0 };
  int deck[MAX_NUMBER_OF_DECKS * MAX_DECK_SIZE] = { 0 };
  int pos_house_hand = 0;
  int pos_player_hand[MAX_PLAYERS] = { 0 };
  int numberOfDecks = 0;
  int initialMoney = 0;
  int betAmount = 0;
  int numberOfCards = 0;
  int currentCard = 0;
  int currentPlayer = 0;
  int i;
  int house_points = 0;
  short turn_ended = 0;

  srand((unsigned) time(NULL)); // initializes the random number generator with current time seed

  /* Prints welcome message */
  printf("**************************\n*                        *\n*  Welcome to BlackJack  *\n*                        *\n**************************\n\n");

  /* Reads parameters */
  ReadGameParameters(&numberOfDecks, &initialMoney, &betAmount);

  /* Gives each player the initial amount of money */
  for (i = 0; i < MAX_PLAYERS; i++) {
      money[i] = initialMoney;
  }

  // initialize graphics
  InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, imgs, &window, &renderer);
  // loads the cards images
  LoadCards(cards);

  numberOfCards = InitializeDeck(deck, numberOfDecks);

  ShuffleDeck(deck, numberOfCards);

  DealCards(deck, &currentCard, player_cards, pos_player_hand, house_cards, &pos_house_hand, money, betAmount);

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
            quit = 1;
            break;
          case SDLK_s:
             // stand !
             if (!turn_ended) {
                 Stand(&currentPlayer, betAmount, money, player_cards, pos_player_hand, player_points);

                 if (currentPlayer >= MAX_PLAYERS) {
                     FinishTurn(deck, numberOfCards, &currentCard, money, betAmount, house_cards, &pos_house_hand, player_points);

                     turn_ended = 1;
                 }
             }

             break;
          case SDLK_h:
            // hit !
            if (!turn_ended) {
                Hit(deck, numberOfCards, &currentCard, player_cards[currentPlayer], &pos_player_hand[currentPlayer], &player_points[currentPlayer]);
                printf("Current Points: %d\n", player_points[currentPlayer]);
                if (Bust(player_points[currentPlayer]) || pos_player_hand[currentPlayer] > MAX_CARD_HAND) {
                    Stand(&currentPlayer, betAmount, money, player_cards, pos_player_hand, player_points);

                    if (currentPlayer >= MAX_PLAYERS) {
                        FinishTurn(deck, numberOfCards, &currentCard, money, betAmount, house_cards, &pos_house_hand, player_points);

                        turn_ended = 1;
                    }
                }
            }

            break;
          case SDLK_n:
            if (turn_ended) {
                if (!DealCards(deck, &currentCard, player_cards, pos_player_hand, house_cards, &pos_house_hand, money, betAmount)) {
                    printf("No more players! Thank you for playing Blackjack!\n");
                    quit = 1;
                    break;
                }
                currentPlayer = -1;

                Stand(&currentPlayer, betAmount, money, player_cards, pos_player_hand, player_points);

                for (i = 0; i < MAX_PLAYERS; i++) {
                    player_points[i] = 0;
                }

                house_points = 0;

                turn_ended = 0;
            }
            break;
          default:
            break;
        }
      }
    }


    // render game table
    RenderTable(money, imgs, renderer, currentPlayer);
    // render house cards
    RenderHouseCards(house_cards, pos_house_hand, cards, renderer);
    // render player cards
    RenderPlayerCards(player_cards, pos_player_hand, cards, renderer);
    // render in the screen all changes above
    SDL_RenderPresent(renderer);
    // add a delay
    SDL_Delay( delay );
  }

  // free memory allocated for images and textures and close everything
  UnLoadCards(cards);
  SDL_FreeSurface(imgs[0]);
  SDL_FreeSurface(imgs[1]);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}

/**
* NextCards: Returns the deck's top card.
* \param deck
* \param currentCard
*/
int *NextCard(int *deck, int *currentCard)
{
    return &deck[++*currentCard];
}

void Hit(int *deck, int numberOfCards, int *currentCard, int *cards, int *pos_hand, int *points)
{
    /* If there are no more cards initializes the deck again */
    if (*currentCard >= numberOfCards) {
        InitializeDeck(deck, (numberOfCards / MAX_DECK_SIZE));
        ShuffleDeck(deck, numberOfCards);

        *currentCard = 0;
    }

    if (*pos_hand <= MAX_CARD_HAND) {
        cards[(*pos_hand)++] = *NextCard(deck, currentCard);

        DeterminePoints(points, cards, *pos_hand);
    }
}

void Stand(int *currentPlayer, int bet, int *money, int player_cards[][MAX_CARD_HAND], int *pos_player_hand, int *player_points)
{
    do {
        if (++(*currentPlayer) < MAX_PLAYERS) {
            DeterminePoints(&player_points[*currentPlayer], player_cards[*currentPlayer], pos_player_hand[*currentPlayer]);
        }
    } while ((money[*currentPlayer] < bet || player_points[*currentPlayer] >= MAXIMUM_POINTS) && *currentPlayer <= MAX_PLAYERS);
}

short Bust(int player_points)
{
    if (player_points > MAXIMUM_POINTS) {
        //TODO: Update UI
        puts("BUST!!!!\f");
        return 1;
    }

    return 0;
}

void FinishTurn(int *deck, int numberOfCards, int *currentCard, int *money, int betAmount, int *house_cards, int *pos_house_hand, int *player_points) {
    int house_points = PlayHouse(house_cards, pos_house_hand, deck, numberOfCards, currentCard);

    UpdateMoney(money, betAmount, player_points, house_points);
}


int PlayHouse(int *house_cards, int *pos_house_hand, int *deck, int numberOfCards, int *currentCard) {
    int housePoints = 0;

    *pos_house_hand = 2;

    DeterminePoints(&housePoints, house_cards, *pos_house_hand);

    while (housePoints < 16) {
        Hit(deck, numberOfCards, currentCard, house_cards, pos_house_hand, &housePoints);
    }

    return housePoints;
}

/**
* InitializeDeck: Initializes an array of card IDs, ordered ascendendly from
* ID 0 to MAX_DECK_SIZE, repeated until numberOfDecks are used. Returns the
* number of cards used.
* \param deck array to be initialized
* \param numberOfDecks number of single decks of cards to be used
*/
int InitializeDeck(int *deck, int numberOfDecks) {
    int numberOfCards = numberOfDecks * MAX_DECK_SIZE;
    int i;

    for (i = 0; i < numberOfCards; i++) {
        deck[i] = i % 52;
    }

    return numberOfCards;
}

/**
* ShuffleDeck: Shuffles the deck of cards using the Fisher-Yates algorithm.
* \param deck array to be shuffled
* \param numberOfCards number of cards in the deck
*/
void ShuffleDeck(int *deck, int numberOfCards)
{
    int i;

    for (i = (numberOfCards - 1); i > 0; i--) {
        int random = rand() % numberOfCards;

        Swap(&deck[i], &deck[random]);
    }
}

/**
* DealCards: Performs the initial deal. Dealing 2 times 1 card for each player.
* Return player dealt cards.
* \param deck deck of cards
* \param currentCard current card of deck
* \param player_cards array with each player's hand
* \param pos_player_hand array with the number of cards of each player
* \param house_cards array with house cards
* \param pos_house_hand number of house cards
*/
short DealCards(int *deck, int *currentCard, int player_cards[][MAX_CARD_HAND], int *pos_player_hand, int *house_cards, int *pos_house_hand, int *money, int bet)
{
    int i, j, cardsDealt = 0;

    for (i = 0; i < INITIAL_CARDS_AMOUNT; i++) {
        for (j = 0; j < MAX_PLAYERS; j++) {
            if (money[j] < bet) {
                pos_player_hand[j] = 0;
                continue;
            }
            player_cards[j][i] = *NextCard(deck, currentCard);
            pos_player_hand[j] = i+1;
            cardsDealt++;
        }

        house_cards[i] = *NextCard(deck, currentCard);
    }

    *pos_house_hand = 1;

    return cardsDealt;
}

/**
* Swap: Swaps a to b.
* \param a
* \param b
*/
void Swap(int *a, int *b)
{
    int auxiliary;

    auxiliary = *a;

    *a = *b;
    *b = auxiliary;
}

void DeterminePoints(int *player_points, int *cards, int pos_player_hand)
{
    int result = 0, numberOfAces = 0, i;

    for (i = 0; i < pos_player_hand; i++) {
        int cardID = cards[i];

        if (IsAce(cardID)) {
            numberOfAces++;
        }

        result += PointsFromCardID(cardID);
    }

    if (result > MAXIMUM_POINTS) {
        for (i = 0; i < numberOfAces; i++) {
            result -= ACE_VALUE_MAX - ACE_VALUE_MIN;

            if (result <= MAXIMUM_POINTS) {
                break;
            }
        }
    }

    *player_points = result;
}

short IsAce(int cardID)
{
    return (cardID % 13) == ACE_ID;
}

int PointsFromCardID(int id)
{
    int cardPosition = id % 13;

    if (cardPosition > 8) {
        return cardPosition == ACE_ID ? ACE_VALUE_MAX : FIGURE_VALUE;
    }

    return cardPosition + 2;
}

void UpdateMoney(int *money, int bet, int *player_points, int house_points)
{
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (money[i] < bet) {
            continue;
        }

        if (house_points > MAXIMUM_POINTS && player_points[i] <= MAXIMUM_POINTS) {
            money[i] += bet * 0.5;
        } else if (player_points[i] > MAXIMUM_POINTS || player_points[i] < house_points) {
            money[i] -= bet;
        } else if (player_points[i] > house_points) {
            money[i] += bet;
        }
    }
}

/**
* ReadGameParameters: Reads the parameters for given variables.
* \param numberOfDecks
* \param initialMoney
* \param betAmount
*/
void ReadGameParameters(int *numberOfDecks, int *initialMoney, int *betAmount)
{
    short check = 1;

    while (check) {
      printf("Please enter the number of decks (%d to %d) you wan't to use in your game: ", MIN_NUMBER_OF_DECKS, MAX_NUMBER_OF_DECKS);
      scanf("%d", numberOfDecks);

      check = *numberOfDecks < MIN_NUMBER_OF_DECKS || *numberOfDecks > MAX_NUMBER_OF_DECKS;

      if (check) {
        printf("\n** Sorry, you have to select between %d to %d decks. **\n\n", MIN_NUMBER_OF_DECKS, MAX_NUMBER_OF_DECKS);
      }
    }

    check = 1;

    while (check) {
      printf("Please enter the initial amount of money for each player (> %d): ", MIN_INITIAL_MONEY);
      scanf("%d", initialMoney);

      check = *initialMoney < MIN_INITIAL_MONEY;

      if (check) {
        printf("\n** Sorry, each player has to have at least 10 EUR. **\n\n");
      }
    }

    check = 1;

    while (check) {
      int betMaximumValue = MAX_BET_VALUE_PERCENTAGE * (*initialMoney);

      printf("Please enter the bet amount (%d to %d): ", MIN_BET_VALUE, betMaximumValue);
      scanf("%d", betAmount);

      check = *betAmount < MIN_BET_VALUE || *betAmount > betMaximumValue;

      if (check) {
        printf("\n** Sorry, bet amount has to be from %d to %d. **\n\n", MIN_BET_VALUE, betMaximumValue);
      }
    }
}

/**
* RenderTable: Draws the table where the game will be played, namely:
* -  some texture for the background
* -  the right part with the IST logo and the student name and number
* -  squares to define the playing positions of each player
* -  names and the available money for each player
* \param _money amount of money of each player
* \param _img surfaces where the table background and IST logo were loaded
* \param _renderer renderer to handle all rendering in a window
*/
void RenderTable(int _money[], SDL_Surface *_img[], SDL_Renderer* _renderer, int currentPlayer)
{
  SDL_Color black = { 0, 0, 0 }; // black
  SDL_Color white = { 255, 255, 255 }; // white
  SDL_Color currentPlayerAreaColor = { 255, 0, 0 }; // red
  char name_money_str[STRING_SIZE];
  TTF_Font *serif = NULL;
  SDL_Texture *table_texture;
  SDL_Rect tableSrc, tableDest, playerRect;
  int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
  int height;

  // set color of renderer to some color
  SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

  // clear the window
  SDL_RenderClear( _renderer );

  // this opens a font style and sets a size
  serif = TTF_OpenFont("FreeSerif.ttf", 16);
  if(!serif)
  {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  tableDest.x = tableSrc.x = 0;
  tableDest.y = tableSrc.y = 0;
  tableSrc.w = _img[0]->w;
  tableSrc.h = _img[0]->h;

  tableDest.w = separatorPos;
  tableDest.h = HEIGHT_WINDOW;

  table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
  SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

  // render the IST Logo
  height = RenderLogo(separatorPos, 0, _img[1], _renderer);

  // render the student name
  height += RenderText(separatorPos+3*MARGIN, height, myName, serif, &black, _renderer);

  // this renders the student number
  RenderText(separatorPos+3*MARGIN, height, myNumber, serif, &black, _renderer);

  // renders the squares + name for each player
  SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );

  // renders the areas for each player: names and money too !
  for ( int i = 0; i < MAX_PLAYERS; i++)
  {
    SDL_Color *areaColor = i == currentPlayer ? &currentPlayerAreaColor : &white;

    playerRect.x = i*(separatorPos/4-5)+10;
    playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
    playerRect.w = separatorPos/4-5;
    playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
    sprintf(name_money_str,"%s -- %d euros", playerNames[i], _money[i]);
    RenderText(playerRect.x+20, playerRect.y-30, name_money_str, serif, areaColor, _renderer);
    SDL_RenderDrawRect(_renderer, &playerRect);
  }

  // destroy everything
  SDL_DestroyTexture(table_texture);

  // close font
  TTF_CloseFont(serif);
}

/**
* RenderHouseCards: Renders cards of the house
* \param _house vector with the house cards
* \param _pos_house_hand position of the vector _house with valid card IDs
* \param _cards vector with all loaded card images
* \param _renderer renderer to handle all rendering in a window
*/
void RenderHouseCards(int _house[], int _pos_house_hand, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
  int card, x, y;
  int div = WIDTH_WINDOW/CARD_WIDTH;

  // drawing all house cards
  for (card = 0; card < _pos_house_hand; card++)
  {
    // calculate its position
    x = (div/2-_pos_house_hand/2+card)*CARD_WIDTH + 15;
    y = (int) (0.26f*HEIGHT_WINDOW);
    // render it !
    RenderCard(x, y, _house[card], _cards, _renderer);
  }
  // just one card ?: draw a card face down
  if (_pos_house_hand == 1)
  {
    x = (div/2-_pos_house_hand/2+1)*CARD_WIDTH + 15;
    y = (int) (0.26f*HEIGHT_WINDOW);
    RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
  }
}

/**
* RenderPlayerCards: Renders the hand, i.e. the cards, for each player
* \param _player_cards 2D array with the player cards, 1st dimension is the player ID
* \param _pos_player_hand array with the positions of the valid card IDs for each player
* \param _cards vector with all loaded card images
* \param _renderer renderer to handle all rendering in a window
*/
void RenderPlayerCards(int _player_cards[][MAX_CARD_HAND], int _pos_player_hand[], SDL_Surface **_cards, SDL_Renderer* _renderer)
{
  int pos, x, y, num_player, card;

  // for every card of every player
  for ( num_player = 0; num_player < MAX_PLAYERS; num_player++)
  {
    for ( card = 0; card < _pos_player_hand[num_player]; card++)
    {
      // draw all cards of the player: calculate its position: only 4 positions are available !
      pos = card % 4;
      x = (int) num_player*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
      y = (int) (0.55f*HEIGHT_WINDOW)+10;
      if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
      if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
      // render it !
      RenderCard(x, y, _player_cards[num_player][card], _cards, _renderer);
    }
  }
}

/**
* RenderCard: Draws one card at a certain position of the window, based on the card code
* \param _x X coordinate of the card position in the window
* \param _y Y coordinate of the card position in the window
* \param _num_card card code that identifies each card
* \param _cards vector with all loaded card images
* \param _renderer renderer to handle all rendering in a window
*/
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
  SDL_Texture *card_text;
  SDL_Rect boardPos;

  // area that will be occupied by each card
  boardPos.x = _x;
  boardPos.y = _y;
  boardPos.w = CARD_WIDTH;
  boardPos.h = CARD_HEIGHT;

  // render it !
  card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
  SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);
}

/**
* LoadCards: Loads all images of the cards
* \param _cards vector with all loaded card images
*/
void LoadCards(SDL_Surface **_cards)
{
  int i;
  char filename[STRING_SIZE];

  // loads all cards to an array
  for (i = 0 ; i < MAX_DECK_SIZE; i++ )
  {
    // create the filename !
    sprintf(filename, ".//cartas//carta_%02d.png", i+1);
    // loads the image !
    _cards[i] = IMG_Load(filename);
    // check for errors: deleted files ?
    if (_cards[i] == NULL)
    {
      printf("Unable to load image: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  }
  // loads the card back
  _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
  if (_cards[i] == NULL)
  {
    printf("Unable to load image: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
}


/**
* UnLoadCards: unloads all card images of the memory
* \param _cards vector with all loaded card images
*/
void UnLoadCards(SDL_Surface **_array_of_cards)
{
  // unload all cards of the memory: +1 for the card back
  for (int i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
  {
    SDL_FreeSurface(_array_of_cards[i]);
  }
}

/**
* RenderLogo function: Renders the IST Logo on the window screen
* \param x X coordinate of the Logo
* \param y Y coordinate of the Logo
* \param _logoIST surface with the IST logo image to render
* \param _renderer renderer to handle all rendering in a window
*/
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
  SDL_Texture *text_IST;
  SDL_Rect boardPos;

  // space occupied by the logo
  boardPos.x = x;
  boardPos.y = y;
  boardPos.w = _logoIST->w;
  boardPos.h = _logoIST->h;

  // render it
  text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
  SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

  // destroy associated texture !
  SDL_DestroyTexture(text_IST);
  return _logoIST->h;
}

/**
* RenderText function: Renders the IST Logo on the window screen
* \param x X coordinate of the text
* \param y Y coordinate of the text
* \param text string where the text is written
* \param font TTF font used to render the text
* \param _renderer renderer to handle all rendering in a window
*/
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
  SDL_Surface *text_surface;
  SDL_Texture *text_texture;
  SDL_Rect solidRect;

  solidRect.x = x;
  solidRect.y = y;
  // create a surface from the string text with a predefined font
  text_surface = TTF_RenderText_Blended(_font,text,*_color);
  if(!text_surface)
  {
    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }
  // create texture
  text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
  // obtain size
  SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
  // render it !
  SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

  SDL_DestroyTexture(text_texture);
  SDL_FreeSurface(text_surface);
  return solidRect.h;
}

/**
* InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
* \param width width in px of the window
* \param height height in px of the window
* \param _img surface to be created with the table background and IST logo
* \param _window represents the window of the application
* \param _renderer renderer to handle all rendering in a window
*/
void InitEverything(int width, int height, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
  InitSDL();
  InitFont();
  *_window = CreateWindow(width, height);
  *_renderer = CreateRenderer(width, height, *_window);

  // load the table texture
  _img[0] = IMG_Load("table_texture.png");
  if (_img[0] == NULL)
  {
    printf("Unable to load image: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // load IST logo
  _img[1] = SDL_LoadBMP("ist_logo.bmp");
  if (_img[1] == NULL)
  {
    printf("Unable to load bitmap: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

}

/**
* InitSDL: Initializes the SDL2 graphic library
*/
void InitSDL()
{
  // init SDL library
  if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
  {
    printf(" Failed to initialize SDL : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
}

/**
* InitFont: Initializes the SDL2_ttf font library
*/
void InitFont()
{
  // Init font library
  if(TTF_Init()==-1)
  {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }
}

/**
* CreateWindow: Creates a window for the application
* \param width width in px of the window
* \param height height in px of the window
* \return pointer to the window created
*/
SDL_Window* CreateWindow(int width, int height)
{
  SDL_Window *window;
  // init window
  window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
  // check for error !
  if ( window == NULL )
  {
    printf("Failed to create window : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  return window;
}

/**
* CreateRenderer: Creates a renderer for the application
* \param width width in px of the window
* \param height height in px of the window
* \param _window represents the window for which the renderer is associated
* \return pointer to the renderer created
*/
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
  SDL_Renderer *renderer;
  // init renderer
  renderer = SDL_CreateRenderer( _window, -1, 0 );

  if ( renderer == NULL )
  {
    printf("Failed to create renderer : %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  // set size of renderer to the same as window
  SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

  return renderer;
}
