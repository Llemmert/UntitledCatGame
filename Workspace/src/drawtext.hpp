#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

void drawText(SDL_Rect *src, SDL_Renderer *ren, const char *text, int x, int y, int r, SDL_Color color)
{
    // UI Text Stuff
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    TTF_Font *Sans = TTF_OpenFont("Workspace/media/fonts/OpenSans-Regular.ttf", 24);
    SDL_Rect text_rect;
    text_surface = TTF_RenderText_Solid(Sans, text, color);
    text_texture = SDL_CreateTextureFromSurface(ren, text_surface);
    text_rect.x = x;
    text_rect.y = y;
    text_rect.w = text_surface->w;
    text_rect.h = text_surface->h;
    SDL_FreeSurface(text_surface);
    SDL_RenderCopy(ren, text_texture, src, &text_rect);
    SDL_DestroyTexture(text_texture);

    // TTF_CloseFont(Sans);
}