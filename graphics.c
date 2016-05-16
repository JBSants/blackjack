#include "graphics.h"
#include "blackjack.h"

const char myName1[] = "Joao Almeida Santos";
const char myNumber1[] = "84083";
const char myName2[] = "Simao Goncalves Eusebio";
const char myNumber2[] = "84183";

int CardGraphicID(Card card) {
    return (card.id + 13 * card.suit) - 1;
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
 * \param currentPlayer
 * \param house_points
 */
void RenderTable(Player_node *players, Player_node *currentPlayer, Player *house, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    SDL_Color currentPlayerAreaColor = { 255, 0, 0 }; // red
    char name_money_str[STRING_SIZE] = { 0 };
    char points_str[STRING_SIZE] = { 0 };
    char house_points_str[STRING_SIZE] = { 0 };
    SDL_Texture *table_texture = NULL;
    SDL_Rect tableSrc = {0, 0, 0, 0}, tableDest = {0, 0, 0, 0}, playerRect = {0, 0, 0, 0};
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height = 0, i = 0;
    SDL_Color *playerColor = NULL; // used for storing player color
    Player_node *player = players;
    
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
    
    // clear the window
    SDL_RenderClear( _renderer );
    
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
    
    // render one student name
    height += RenderText(separatorPos+3*MARGIN, height, myName1, _font, &black, _renderer);
    
    // this renders one student number
    height += RenderText(separatorPos+3*MARGIN, height, myNumber1, _font, &black, _renderer);
    
    // render another student name
    height += RenderText(separatorPos+3*MARGIN, height, myName2, _font, &black, _renderer);
    
    // this renders another student number
    height += RenderText(separatorPos+3*MARGIN, height, myNumber2, _font, &black, _renderer) + 20;
    
    /* Checks if house_points is not 0. If so, renders, above the first player,
     * the house points
     */
    if (i == 0 && house->score > 0) {
        sprintf(house_points_str, "House score: %d", house->score);
        if (house->score > 21) {
            strcat(house_points_str, " (BUST)");
        }
        RenderText(35, (0.55f*HEIGHT_WINDOW)-130, house_points_str, _font, &white, _renderer);
    }
    
    // renders the areas for each player: names and money too !
    while (player != NULL)
    {
        /* Determines this player color. If this player is current player player color is red. */
        playerColor = player == currentPlayer ? &currentPlayerAreaColor : &white;
        
        /* If this is the current player, show red rect */
        if (player == currentPlayer) {
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255 );
        } else {
            SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
        }
        
        playerRect.x = player->player.position*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        
        sprintf(name_money_str,"%s -- Bet: %.2f euros", player->player.name, player->player.bet);
        
        sprintf(points_str, "%d points", player->player.score);
        
        /* If player is bust */
        if (Bust(player->player)) {
            strcat(points_str, " (BUST)"); // if bust concatnate BUST to string
        }
        
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, playerColor, _renderer);
        RenderText(playerRect.x+20, playerRect.y-30, points_str, _font, playerColor, _renderer); // renders points
        
        sprintf(name_money_str, "%s", player->player.name);
        
        if (player->player.ai) {
            strcat(name_money_str, " (Computer)");
        } else {
            strcat(name_money_str, " (Human)");
        }
        
        height += RenderText(separatorPos+3*MARGIN, height, name_money_str, _font, &black, _renderer);
        
        sprintf(name_money_str, "Money: %.2f", player->player.money);
        
        height += RenderText(separatorPos+3*MARGIN, height, name_money_str, _font, &black, _renderer) + 5;
        
        SDL_RenderDrawRect(_renderer, &playerRect);
        i += 1;
        
        player = player->next;
    }
    
    // destroy everything
    SDL_DestroyTexture(table_texture);
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(Player *house, Card_node *cards_head, int cardNumber, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    if (cards_head != NULL) {
        int x = 0, y = 0;
        int div = WIDTH_WINDOW/CARD_WIDTH;
        
        if (house->hand_size > 1) {
            RenderHouseCards(house, cards_head->next, cardNumber - 1, _cards, _renderer);
        }
        
        x = (div/2-(house->hand_size)/2+(cardNumber - 1))*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        
        // render it !
        RenderCard(x, y, CardGraphicID(cards_head->card), _cards, _renderer);
        
        // just one card ?: draw a card face down
        if (house->hand_size == 1)
        {
            x = (div/2-(house->hand_size)/2+1)*CARD_WIDTH + 15;
            y = (int) (0.26f*HEIGHT_WINDOW);
            RenderCard(x, y, 52, _cards, _renderer);
        }
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(Player_node *players, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    Player_node *player_node = players;
    
    // for every card of every player
    while (player_node != NULL)
    {
        
        RenderCards(player_node->player.hand_size, player_node->player.position, player_node->player.cards, _cards, _renderer);
        
        player_node = player_node->next;
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
void RenderCards(int cardNumber, int playerNumber, Card_node *cards_head, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    if (cards_head != NULL) {
        int pos = 0, x = 0, y = 0;
        
        RenderCards(cardNumber - 1, playerNumber, cards_head->next, _cards, _renderer);
        
        pos = (cardNumber - 1) % 4;
        x = (int) playerNumber*((0.95f*WIDTH_WINDOW)/4-5)+((cardNumber - 1)/4)*12+15;
        y = (int) (0.55f*HEIGHT_WINDOW)+10;
        if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
        if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
        
        RenderCard(x, y, CardGraphicID(cards_head->card), _cards, _renderer);
    }
}

void RenderCard(int x, int y, int card_id, SDL_Surface **_cards, SDL_Renderer* _renderer) {
    SDL_Texture *card_text = NULL;
    SDL_Rect boardPos = {0, 0, 0, 0};
    
    // area that will be occupied by each card
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;
    
    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[card_id]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);
    
    SDL_DestroyTexture(card_text);
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i = 0;
    char filename[STRING_SIZE] = { 0 };
    
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
    SDL_Texture *text_IST = NULL;
    SDL_Rect boardPos  = {0, 0, 0, 0};
    
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
    SDL_Surface *text_surface = NULL;
    SDL_Texture *text_texture = NULL;
    SDL_Rect solidRect = {0, 0, 0, 0};
    
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
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
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
    
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
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
    SDL_Window *window = NULL;
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
    SDL_Renderer *renderer = NULL;
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
