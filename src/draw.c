/*
 * PrincessEdit
 *
 * Copyright (C) 2015 NovaSquirrel
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

SDL_Color FGColor = {0x00, 0x00, 0x00, 255};
SDL_Color BGColor = {0xf0, 0xf0, 0xf0, 255};
SDL_Color SelectColor = {0xff, 0x00, 0xff, 255};
SDL_Color AvailableColor = {0x00, 0xff, 0xff, 255};
SDL_Color GridColor = {0xc0, 0xc0, 0xc0, 255};

// drawing functions
void rectfill(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2) {
  SDL_Rect Temp = {X1, Y1, abs(X2-X1)+1, abs(Y2-Y1)+1};
  SDL_RenderFillRect(Bmp,  &Temp);
}
void rect(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2) {
  SDL_Rect Temp = {X1, Y1, abs(X2-X1)+1, abs(Y2-Y1)+1};
  SDL_RenderDrawRect(Bmp, &Temp);
}
void sblit(SDL_Surface* SrcBmp, SDL_Surface* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height) {
  SDL_Rect Src = {SourceX, SourceY, Width, Height};
  SDL_Rect Dst = {DestX, DestY};
  SDL_BlitSurface(SrcBmp, &Src, DstBmp, &Dst);
}
void blit(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height) {
  SDL_Rect Src = {SourceX, SourceY, Width, Height};
  SDL_Rect Dst = {DestX, DestY, Width, Height};
  SDL_RenderCopy(DstBmp,  SrcBmp, &Src, &Dst);
}
void blitf(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, SDL_RendererFlip Flip) {
  SDL_Rect Src = {SourceX, SourceY, Width, Height};
  SDL_Rect Dst = {DestX, DestY, Width, Height};
  SDL_RenderCopyEx(DstBmp,  SrcBmp, &Src, &Dst, 0, NULL, Flip);
}
void blitz(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Width2, int Height2) {
  SDL_Rect Src = {SourceX, SourceY, Width, Height};
  SDL_Rect Dst = {DestX, DestY, Width2, Height2};
  SDL_RenderCopy(DstBmp,  SrcBmp, &Src, &Dst);
}
void blitfull(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int DestX, int DestY) {
  SDL_Rect Dst = {DestX, DestY};
  SDL_RenderCopy(DstBmp,  SrcBmp, NULL, &Dst);
}

void Free_FontSet(FontSet *Fonts) {
  if(Fonts->Font[0]) TTF_CloseFont(Fonts->Font[0]);
  if(Fonts->Font[1]) TTF_CloseFont(Fonts->Font[1]);
  if(Fonts->Font[2]) TTF_CloseFont(Fonts->Font[2]);
  if(Fonts->Font[3]) TTF_CloseFont(Fonts->Font[3]);
}

int Load_FontSet(FontSet *Fonts, int Size, const char *Font1, const char *Font2, const char *Font3, const char *Font4) {
  Fonts->Font[0] = NULL; Fonts->Font[1] = NULL;
  Fonts->Font[2] = NULL; Fonts->Font[3] = NULL;
  Fonts->Width = 0; Fonts->Height = 0;

  Fonts->Font[0] = TTF_OpenFont(Font1, Size);
  Fonts->Font[1] = TTF_OpenFont(Font2, Size);
  Fonts->Font[2] = TTF_OpenFont(Font3, Size);
  Fonts->Font[3] = TTF_OpenFont(Font4, Size);

  if(!Fonts->Font[0] || !Fonts->Font[1] || !Fonts->Font[2] || !Fonts->Font[3]) {
    SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "Error loading one or more fonts - SDL_ttf Error: %s", TTF_GetError());
    Free_FontSet(Fonts);
    return 0;
  }

  TTF_SizeText(Fonts->Font[0], "N", &Fonts->Width, &Fonts->Height);
  return 1;
}

int RenderRotateText(SDL_Renderer *Renderer, FontSet *Font, int X, int Y, int Flags, double Rotation, const char *Text) {
  SDL_Surface *TextSurface2 = TTF_RenderUTF8_Shaded(Font->Font[Flags&3],Text,FGColor,BGColor);
  SDL_Surface *TextSurface = SDL_ConvertSurfaceFormat(TextSurface2, SDL_PIXELFORMAT_RGBA8888, 0);

  SDL_Surface *FormatSurface=TTF_RenderUTF8_Shaded(Font->Font[Flags&3],"cbiupr\xe2\x86\xb5",FGColor,BGColor);
  SDL_Texture *Texture;

  char FormatCodes[] = {0x03,0x02,0x1d,0x1f,0x0f,0x16,'\n',0};
  unsigned char *Text2 = (unsigned char*)Text;
  int CurChar = 0;
  while(*Text2) {
    char *Which = strchr(FormatCodes, (char)Text2[0]);
    if(Which)
      sblit(FormatSurface, TextSurface, (Which-FormatCodes)*Font->Width, 0, CurChar*Font->Width, 0, Font->Width, Font->Height);
    if(*Text2<0x80||*Text2>0xbf)
      CurChar++;
    Text2++;
  }

  Texture = SDL_CreateTextureFromSurface(Renderer, TextSurface);

  SDL_Rect Src = {0, 0, TextSurface->w, TextSurface->h};
  SDL_Rect Dst = {X, Y, TextSurface->w, TextSurface->h};
  SDL_Point Center = {Font->Width/2, TextSurface->h/2};
  SDL_RenderCopyEx(Renderer, Texture,  &Src, &Dst, Rotation, &Center, SDL_FLIP_NONE);
  SDL_FreeSurface(TextSurface);
  SDL_FreeSurface(TextSurface2);
  SDL_FreeSurface(FormatSurface);
  SDL_DestroyTexture(Texture);
  return 1;
}

int RenderSimpleText(SDL_Renderer *Renderer, FontSet *Font, int X, int Y, int Flags, const char *Text) {
  if(!*Text) return 1;
  SDL_Surface *TextSurface2 = TTF_RenderUTF8_Shaded(Font->Font[Flags&3],Text,FGColor,BGColor);
  SDL_Surface *TextSurface = SDL_ConvertSurfaceFormat(TextSurface2, SDL_PIXELFORMAT_RGBA8888, 0);

  SDL_Surface *FormatSurface=TTF_RenderUTF8_Shaded(Font->Font[Flags&3],"cbiupr\xe2\x86\xb5",FGColor,BGColor);
  SDL_Texture *Texture;

  char FormatCodes[] = {0x03,0x02,0x1d,0x1f,0x0f,0x16,'\n',0};
  unsigned char *Text2 = (unsigned char*)Text;
  int CurChar = 0;
  while(*Text2) {
    char *Which = strchr(FormatCodes, (char)Text2[0]);
    if(Which)
      sblit(FormatSurface, TextSurface, (Which-FormatCodes)*Font->Width, 0, CurChar*Font->Width, 0, Font->Width, Font->Height);
    if(*Text2<0x80||*Text2>0xbf)
      CurChar++;
    Text2++;
  }

  Texture = SDL_CreateTextureFromSurface(Renderer, TextSurface);
  blit(Texture, Renderer, 0, 0, X, Y, TextSurface->w, TextSurface->h);
  SDL_FreeSurface(TextSurface);
  SDL_FreeSurface(TextSurface2);
  SDL_FreeSurface(FormatSurface);
  SDL_DestroyTexture(Texture);
  return 1;
}
