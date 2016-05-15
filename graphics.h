#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include "cards.h"
#include "players.h"

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

void InitEverything(int , int, TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(Player_node *players, Player_node *currentPlayer, Player *house, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer);
void RenderCards(int cardNumber, int playerNumber, Card_node *cards_head, SDL_Surface **_cards, SDL_Renderer* _renderer);
void RenderCard(int x, int y, int card_id, SDL_Surface **_cards, SDL_Renderer* _renderer);
void RenderHouseCards(Player *house, Card_node *cards_head, int cardNumber, SDL_Surface **_cards, SDL_Renderer* _renderer);
void RenderPlayerCards(Player_node *players, SDL_Surface **_cards, SDL_Renderer* _renderer);
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);

#endif
