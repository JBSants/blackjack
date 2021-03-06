#include "graphics.h"
#include "blackjack.h"

/**
 * CardGraphicID: Returns card number used by the graphics functions
 * \param card
 */
int CardGraphicID(Card card) {
    return (card.id + 13 * card.suit) - 1;
}

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param players players list
 * \param currentPlayer
 * \param house
 * \param _font font used to render text
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(Player_node *players, Player_node *currentPlayer, Player *house, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer)
{
    SDL_Color black = { 0, 0, 0 }; // Black
    SDL_Color white = { 255, 255, 255 }; // White
    SDL_Color currentPlayerAreaColor = { 255, 0, 0 }; // Red
    char name_money_str[STRING_SIZE] = { 0 };
    char points_str[STRING_SIZE] = { 0 };
    char house_points_str[STRING_SIZE] = { 0 };
    SDL_Texture *table_texture = NULL;
    SDL_Rect tableSrc = {0, 0, 0, 0}, tableDest = {0, 0, 0, 0}, playerRect = {0, 0, 0, 0};
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // Seperates the left from the right part of the window
    int height = 0, i = 0;
    SDL_Color *playerColor = NULL; // Used for storing player color
    Player_node *player = players;
    
    // Set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
    
    // Clear the window
    SDL_RenderClear( _renderer );
    
    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    
    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;
    
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);
    
    // Render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);
    
    // Render one student name
    height += RenderText(separatorPos+3*MARGIN, height, NAME_JOAO, _font, &black, _renderer);
    
    // This renders one student number
    height += RenderText(separatorPos+3*MARGIN, height, NUMBER_JOAO, _font, &black, _renderer);
    
    // Render another student name
    height += RenderText(separatorPos+3*MARGIN, height, NAME_SIMAO, _font, &black, _renderer);
    
    // This renders another student number
    height += RenderText(separatorPos+3*MARGIN, height, NUMBER_SIMAO, _font, &black, _renderer) + 20;
    
    /* Checks if house_points is not 0. If so, renders, above the first player,
     * the house points
     */
    if (i == 0 && house->score > 0) {
        if (Bust((*house))) {
            sprintf(house_points_str, "House score: BUST!");
        } else {
            sprintf(house_points_str, "House score: %d", house->score);
        }
        
        RenderText(35, (0.55f*HEIGHT_WINDOW)-130, house_points_str, _font, Bust((*house)) ? &currentPlayerAreaColor : &white, _renderer); // Renders it
    }
    
    /* If there is no more players, show a message */
    if (empty(player)) {
        RenderText(0.4f * separatorPos, 0.75f * HEIGHT_WINDOW, "No more players! Press 'a' to add.", _font, &white, _renderer);
    }
    
    // Renders the areas for each player: names and money too !
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
        
        
        
        /* Checks bust */
        if (Bust(player->player)) {
            sprintf(points_str, "BUST!"); // If so, displays BUST!
        } else {
            sprintf(points_str, "%d points", player->player.score); // If not, displays score
        }
        
        RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, playerColor, _renderer); // renders name and bet
        RenderText(playerRect.x+20, playerRect.y-30, points_str, _font, Bust(player->player) ? &currentPlayerAreaColor : playerColor, _renderer); // renders points
        
        sprintf(name_money_str, "%s", player->player.name);
        
        /* Checks if player is ai */
        if (player->player.ai) {
            strcat(name_money_str, " (Computer)");
        } else {
            strcat(name_money_str, " (Human)");
        }
        
        height += RenderText(separatorPos+3*MARGIN, height, name_money_str, _font, &black, _renderer); // Renders player info on the side bar
        
        sprintf(name_money_str, "Money: %.2f", player->player.money);
        
        height += RenderText(separatorPos+3*MARGIN, height, name_money_str, _font, &black, _renderer) + 5; // Renders player money on the side bar
        
        SDL_RenderDrawRect(_renderer, &playerRect);
        i += 1;
        
        player = player->next;
    }
    
    // Destroy everything
    SDL_DestroyTexture(table_texture);
}

/**
 * GetPlayerPositionFromXY: Returns clicked player position
 * \param x
 * \param y
 */
int GetPlayerPositionFromXY(int x, int y) {
    /* Checks if the click is valid */
    if (y < 0.55f * HEIGHT_WINDOW) {
        return -1; // If not, return -1
    }
    
    return (int) (x + 10)/((0.95f * WIDTH_WINDOW)/4-5);
}

/**
 * DisplayAddPlayerHover: Renders a blue rectangle under hovered player area, if empty
 * \param x
 * \param y
 * \param players players list
 * \param _renderer
 */
void DisplayAddPlayerHover(int x, int y, Player_node *players, SDL_Renderer *_renderer) {
    int playerPosition = GetPlayerPositionFromXY(x, y); // Hovered position
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    Player_node *player_node_aux = players; // Auxiliary variable for walking players list
    SDL_Rect *playerRect = NULL; // Rectangle to be rendered
    
    /* Checks if player position is valid  (between min and max) */
    if (playerPosition >= (MIN_PLAYERS - 1) && playerPosition < MAX_PLAYERS) {
        /* Walks player list */
        while (player_node_aux != NULL) {
            /* If position is already taken return, not rendering the rect */
            if (playerPosition == player_node_aux->player.position) {
                return;
            }
            
            player_node_aux = player_node_aux->next;
        }
        
        playerRect = (SDL_Rect *) malloc(sizeof(SDL_Rect)); // Allocates the rectangle
        
        /* Checks malloc */
        if (playerRect == NULL) {
            ALLOCATION_ERROR_MESSAGE();
            
            exit(EXIT_FAILURE);
        }
        
        playerRect->x = playerPosition*(separatorPos/4-5)+10;
        playerRect->y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect->w = separatorPos/4-5;
        playerRect->h = (int) (0.42f*HEIGHT_WINDOW);
        
        SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
        
        SDL_RenderDrawRect(_renderer, playerRect);
        
        free(playerRect);
    }
}

/**
 * DisplayAddPlayerMessage: Renders a message informing 'a' option is selected
 * \param _renderer
 * \param _font
 */
void DisplayAddPlayerMessage(SDL_Renderer *_renderer, TTF_Font *_font) {
    SDL_Color *white = (SDL_Color *) malloc(sizeof(SDL_Color)); // Allocates color
    
    /* Checks malloc */
    if (white == NULL) {
        ALLOCATION_ERROR_MESSAGE();
        
        exit(EXIT_FAILURE);
    }
    
    /* Sets white color */
    white->a = 255;
    white->r = 255;
    white->g = 255;
    white->b = 255;
    
    RenderText(35, 0.35F * HEIGHT_WINDOW, "Adding new player", _font, white, _renderer);
    
    free(white);
}

/**
 * DisplayBetMessage: Renders a message informing 'b' option is selected
 * \param _renderer
 * \param _font
 */
void DisplayBetMessage(SDL_Renderer *_renderer, TTF_Font *_font) {
    SDL_Color *white = (SDL_Color *) malloc(sizeof(SDL_Color)); // Allocates color
    
    /* Checks malloc */
    if (white == NULL) {
        ALLOCATION_ERROR_MESSAGE();
        
        exit(EXIT_FAILURE);
    }
    
    /* Sets white color */
    white->a = 255;
    white->r = 255;
    white->g = 255;
    white->b = 255;
    
    RenderText(35, 0.35F * HEIGHT_WINDOW, "Bet", _font, white, _renderer);
    
    free(white);
}

/**
 * RenderHouseCards: Renders cards of the house.
 * \param _house
 * \param cards_head list of house cards
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(Player *house, Card_node *cards_head, int cardNumber, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    /* Checks if current card head isn't null */
    if (cards_head != NULL) {
        int x = 0, y = 0; // Card position
        int div = WIDTH_WINDOW / CARD_WIDTH;
        
        /* If there are more cards to render */
        if (house->hand_size > 1) {
            /* Renders next card first */
            RenderHouseCards(house, cards_head->next, cardNumber - 1, _cards, _renderer);
        }
        
        x = (div/2-(house->hand_size)/2+(cardNumber - 1))*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        
        // Render it !
        RenderCard(x, y, CardGraphicID(cards_head->card), _cards, _renderer);
        
        // Just one card ?: draw a card face down
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
 * \param players players list
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(Player_node *players, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    Player_node *player_node = players; // auxiliary variable used for walking the list
    
    // for every card of every player
    while (player_node != NULL)
    {
        
        RenderCards(player_node->player.hand_size, player_node->player.position, player_node->player.cards, _cards, _renderer); // Renders cards!
        
        player_node = player_node->next;
    }
}

/**
 * RenderCards: Draws all players cards
 * \param cardNumber number of the card to be drawn
 * \param playerNumber player position
 * \param card_head current card list head
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCards(int cardNumber, int playerNumber, Card_node *cards_head, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    /* Checks if current card head isn't null */
    if (cards_head != NULL) {
        int pos = 0, x = 0, y = 0; // Position vars
        
        /* Renders next card first! */
        RenderCards(cardNumber - 1, playerNumber, cards_head->next, _cards, _renderer);
        
        /* Determines position */
        pos = (cardNumber - 1) % 4;
        x = (int) playerNumber*((0.95f*WIDTH_WINDOW)/4-5)+((cardNumber - 1)/4)*12+15;
        y = (int) (0.55f*HEIGHT_WINDOW)+10;
        if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
        if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
        
        RenderCard(x, y, CardGraphicID(cards_head->card), _cards, _renderer); // Render it
    }
}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param x X coordinate of the card position in the window
 * \param y Y coordinate of the card position in the window
 * \param card_id card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCard(int x, int y, int card_id, SDL_Surface **_cards, SDL_Renderer* _renderer) {
    SDL_Texture *card_text = NULL;
    SDL_Rect boardPos = {0, 0, 0, 0};
    
    // Area that will be occupied by each card
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;
    
    // Render it !
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
 * \param _font TTF font used to render the text
 * \param _color color used for rendering
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
 * \param _font TTF font to be initialized
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
