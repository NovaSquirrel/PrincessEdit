/*
 * PrincessEdit
 *
 * Copyright (C) 2016 NovaSquirrel
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "editor.h"

char *InputLine(const char *Prompt, char *Buffer, int BufferSize) {
  int Height = MainFont.Height + 4;
  SDL_Rect Rect = {10, ScreenHeight/2-(Height*2)/2, ScreenWidth-20, (Height*2)};

  int End = 0, ReRender = 1, CursorX = 0, Canceled = 0;

  while(!End) {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT)
        quit = End = 1;
      else if(e.type == SDL_TEXTINPUT) {
        char Character = e.text.text[0];
        if(Buffer[CursorX])
          memmove(Buffer+CursorX+1, Buffer+CursorX, strlen(Buffer+CursorX)+1);
        else
          Buffer[CursorX+1] = 0;
        Buffer[CursorX++] = Character;
        ReRender = 1;
      }
      else if(e.type == SDL_KEYDOWN) {
        if(e.key.keysym.sym == SDLK_LEFT && CursorX) {
          CursorX--;
          ReRender = 1;
        }
        else if(e.key.keysym.sym == SDLK_RIGHT && Buffer[CursorX]) {
          CursorX++;
          ReRender = 1;
        }
        else if(e.key.keysym.sym == SDLK_BACKSPACE && CursorX) {
          CursorX--;
          memmove(Buffer+CursorX, Buffer+CursorX+1, strlen(Buffer+CursorX)+1);
          ReRender = 1;
        }
        else if(e.key.keysym.sym == SDLK_DELETE && Buffer[CursorX]) {
          memmove(Buffer+CursorX, Buffer+CursorX+1, strlen(Buffer+CursorX)+1);
          ReRender = 1;
        }
        else if(e.key.keysym.sym == SDLK_RETURN) {
          End = 1;
        }
        else if(e.key.keysym.sym == SDLK_ESCAPE) {
          End = 1;
          Canceled = 1;
        }
      }
    }
    if(ReRender) {
      // Make box
      SDL_Rect Outline = {Rect.x-1, Rect.y-1, Rect.w+2, Rect.h+2};
      SDL_SetRenderDrawColor(ScreenRenderer, FGColor.r, FGColor.g, FGColor.b, 255);
      SDL_RenderDrawRect(ScreenRenderer, &Outline);
      SDL_SetRenderDrawColor(ScreenRenderer, BGColor.r, BGColor.g, BGColor.b, 255);
      SDL_RenderFillRect(ScreenRenderer, &Rect);

      // Add text
      RenderSimpleText(ScreenRenderer, &MainFont, Rect.x+4, Rect.y, 1, Prompt);
      RenderSimpleText(ScreenRenderer, &MainFont, Rect.x+4, Rect.y+Height, 0, Buffer);

      // Add cursor
      SDL_SetRenderDrawColor(ScreenRenderer, 0, 128, 0, 255);
      SDL_RenderDrawLine(ScreenRenderer, Rect.x+4+CursorX*MainFont.Width, Rect.y+Height, Rect.x+4+CursorX*MainFont.Width, Rect.y+Height+MainFont.Height);

      SDL_RenderPresent(ScreenRenderer);
      ReRender = 0;
    }
    SDL_Delay(17);
  }
  return (Canceled)?NULL:Buffer;
}
