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

SDL_Window *window = NULL;
SDL_Renderer *ScreenRenderer = NULL;
SDL_Texture *PrincessIcon = NULL;
SDL_Surface *WindowIcon = NULL;
int ScreenWidth = 800, ScreenHeight = 600;
int Redraw = 1, RedrawMap = 1;
FontSet MainFont, TinyFont, VeryTinyFont;
int MapViewX = 10, MapViewY = 10, MapViewWidth = 20, MapViewHeight = 20, MapViewWidthP, MapViewHeightP;
int TileW = 16, TileH = 16, CameraX = 0, CameraY = 0;
int CursorX, CursorY, CursorShown = 0;
int CurLayer = 0, TilePicker = 0, TileOptions = 0;
int JustToggledTP = 0, JustToggledOptions = 0;
int GridOn = 1;

LevelRect TmpLevelRect;  // for copying from into the level
LevelRect *CurLevelRect = NULL; // selected level rectangle
LevelRect *AvailableRect = NULL; // available level rectangle
int TPW = 0, TPH = 0, TPCursorX = 0, TPCursorY = 0, TPCursorShown = 0;
SDL_Rect TPRect = {0, 0, 0, 0};
const char *ShiftNumbers = "!@#$%^&*()"; // Europe doesn't exist
SDL_Cursor *SystemCursors[SYSCURSOR_MAX];

int SetACursor;
int WhatCursor = 0;
int DraggingMove = 0;
int DraggingSelect = 0;
int DraggingResize = 0; int ResizeUp=0, ResizeLeft=0, ResizeRight=0, ResizeDown=0;

const char *RectangleTypeNames[] = {
  "Simple",
  "Slope",
  "Bitmap",
  "Custom"
};

void GUI_SetCursor(int CursorNum) {
  if(CursorNum != WhatCursor)
    SDL_SetCursor(SystemCursors[CursorNum]);
  WhatCursor = CursorNum;
  SetACursor = 1;
};

SDL_Surface *SDL_LoadImage(const char *FileName, int Flags) {
  SDL_Surface* loadedSurface = IMG_Load(FileName);
  if(loadedSurface == NULL) {
    SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", window, "Unable to load image %s! SDL Error: %s", FileName, SDL_GetError());
    return NULL;
  }
  if(Flags & 1)
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));
  return loadedSurface;
}
SDL_Texture *LoadTexture(const char *FileName, int Flags) {
  SDL_Surface *Surface = SDL_LoadImage(FileName, Flags);
  if(!Surface) return NULL;
  SDL_Texture *Texture = SDL_CreateTextureFromSurface(ScreenRenderer, Surface);
  SDL_FreeSurface(Surface);
  return Texture;
}

SDL_Rect MakeSelectRect(LevelRect *Rect, int Offset) {
  SDL_Rect Select = {MapViewX+(Rect->X-CameraX)*TileW-Offset, MapViewY+(Rect->Y-CameraY)*TileH-Offset, Rect->W*TileW+Offset*2, Rect->H*TileH+Offset*2};
  return Select;
}

int UpdateResizeDirections() {
  int MouseX, MouseY;
  SDL_Rect Select1 = MakeSelectRect(CurLevelRect, 12);
  SDL_Rect Select2 = MakeSelectRect(CurLevelRect, 0);
  SDL_GetMouseState(&MouseX, &MouseY);
  if(IsInsideRect(MouseX, MouseY, Select1.x, Select1.y, Select1.w, Select1.h) &&
    !IsInsideRect(MouseX, MouseY, Select2.x, Select2.y, Select2.w, Select2.h)) {
    ResizeUp    = MouseY < Select2.y;
    ResizeDown  = MouseY > (Select2.y + Select2.h);
    ResizeLeft  = MouseX < Select2.x;
    ResizeRight = MouseX > (Select2.x + Select2.w);
    return 1;
  }
  return 0;
}

void GUIClick(int Button) {
  if(Button == SDL_BUTTON_LEFT) {
    if(TilePicker) {
      if(TPCursorShown) {
        TmpLevelRect.Type = TilesetLookupIdToIndex(CurLayer, (TPCursorY<<8)|TPCursorX);
        if(TmpLevelRect.Type == -1) {
          CurLevelRect = NULL;
          return;
        }
        TmpLevelRect.W = TmpLevelRect.H = 1;
        if(TmpLevelRect.ExtraInfo)
          free(TmpLevelRect.ExtraInfo);
        TmpLevelRect.ExtraInfo = NULL;
        CurLevelRect = &TmpLevelRect;

        TilePicker = 0;
        JustToggledTP = 1;
        Redraw = 1;
      }
    } else {
      if(CursorShown) {
        int RealX = CursorX+CameraX, RealY = CursorY+CameraY;
        if(LayerInfos[CurLayer].Type == LAYER_TILEMAP) {
          TmpLevelRect.Type = TilesetLookupIdToIndex(CurLayer, LayerInfos[CurLayer].Map[RealY*LayerInfos[CurLayer].LayerWidth+RealX].Graphic);
          TmpLevelRect.W = TmpLevelRect.H = 1;
          if(TmpLevelRect.ExtraInfo)
            free(TmpLevelRect.ExtraInfo);
          TmpLevelRect.ExtraInfo = NULL;
          CurLevelRect = &TmpLevelRect;
        } else {
          if(CurLevelRect) {
            int MouseX, MouseY;
            SDL_GetMouseState(&MouseX, &MouseY);
            if(UpdateResizeDirections())
              return;
          }
          CurLevelRect = LayerInfos[CurLayer].Map[RealY*LayerInfos[CurLayer].LayerWidth+RealX].Rect;
          DraggingMove = 1;
        }
        Redraw = 1;
      }
    }
  } else if(Button == SDL_BUTTON_RIGHT) {
    if(TilePicker) {
      if(TPCursorShown) {
      }
    } else {
      if(CursorShown && CurLevelRect) {
        int RealX = CursorX+CameraX, RealY = CursorY+CameraY;
        if(LayerInfos[CurLayer].Type == LAYER_TILEMAP) {
          int NewId = (CurLevelRect->Type>=0)?LayerInfos[CurLayer].TilesetLookup[CurLevelRect->Type].Id:-1;
          LayerInfos[CurLayer].Map[RealY*LayerInfos[CurLayer].LayerWidth+RealX].Graphic = NewId;
          RedrawMap = 1;
        } else {
          LevelRect *End = LayerInfos[CurLayer].Rects;
          if(End)
            while(End->Next)
              End = End->Next;

          LevelRect *New = (LevelRect*)malloc(sizeof(LevelRect));
          *New = *CurLevelRect;
          New->Prev = End;
          New->Next = NULL;
          New->X = RealX;
          New->Y = RealY;

          if(End)
            End->Next = New;
          else
            LayerInfos[CurLayer].Rects = New;
          RedrawMap = 2;
        }
        Redraw = 1;
      }
    }
  }
}

void StartGUI() {
  char Temp[100];
  int n;
  sprintf(Temp, "PrincessEdit - %s", LevelFilename);
  memset(&TmpLevelRect, 0, sizeof(TmpLevelRect));

  // tile picker stuff
  SDL_Texture *TPTexture;

  window = SDL_CreateWindow(Temp, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN| SDL_WINDOW_RESIZABLE);
  if(!window) {
     SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "Window could not be created! SDL_Error: %s", SDL_GetError());
     return;
  }
  if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
    SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
    return;
  }
  if( TTF_Init() == -1 ) {
    SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
    return;
  }

  ScreenRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
  if(!SDL_RenderTargetSupported(ScreenRenderer)) {
    SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "Rendering to a texture isn't supported");
    return;
  }

  // set window icon
  WindowIcon = SDL_LoadImage("data/icon.png", 0);
  SDL_SetWindowIcon(window, WindowIcon);

  // load cursors
  SystemCursors[SYSCURSOR_NORMAL] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  SystemCursors[SYSCURSOR_WAIT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
  SystemCursors[SYSCURSOR_SIZE_NS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
  SystemCursors[SYSCURSOR_SIZE_EW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
  SystemCursors[SYSCURSOR_SIZE_NWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
  SystemCursors[SYSCURSOR_SIZE_NESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
  SystemCursors[SYSCURSOR_SIZE_ALL] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
  SystemCursors[SYSCURSOR_HAND] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
  SystemCursors[SYSCURSOR_DISABLE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

  if (!Load_FontSet(&MainFont, 14, "data/font/font.ttf", "data/font/fontb.ttf", "data/font/fonti.ttf", "data/font/fontbi.ttf"))
    return;
  if (!Load_FontSet(&TinyFont, 10, "data/font/font.ttf", "data/font/fontb.ttf", "data/font/fonti.ttf", "data/font/fontbi.ttf"))
    return;
  if (!Load_FontSet(&VeryTinyFont, 8, "data/font/font.ttf", "data/font/fontb.ttf", "data/font/fonti.ttf", "data/font/fontbi.ttf"))
    return;
  LoadTilesets();

// all initialized

  AutoloadDirectory("data/scripts/");

  while(!quit) {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT)
        quit = 1;
      else if (e.type == SDL_DROPFILE) { // for loading plugins eventually
        char *dropped_filedir = e.drop.file;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window", dropped_filedir, NULL);
        SDL_free(dropped_filedir);
        break;
      } else if(e.type == SDL_MOUSEMOTION) {
        SetACursor = 0;
        AvailableRect = NULL;
        if(!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
          DraggingMove = 0;
          DraggingResize = 0;
        }
        if(!TilePicker) {
          if(DraggingResize || IsInsideRect(e.motion.x, e.motion.y, MapViewX, MapViewY, MapViewWidthP, MapViewHeightP)) {
            int NewX = (e.motion.x - MapViewX) / TileW;
            int NewY = (e.motion.y - MapViewY) / TileH;
            if((e.motion.x - MapViewX) % TileW > TileW/2)
              NewX++;
            if((e.motion.y - MapViewY) % TileH > TileH/2)
              NewY++;
            if(NewY < 0) continue;
            if(NewX != CursorX || NewY != CursorY || !CursorShown) {
              Redraw = 1;
              int DiffX = NewX-CursorX;
              int DiffY = NewY-CursorY;
              if(DraggingMove && CurLevelRect) {
                CurLevelRect->X += DiffX;
                CurLevelRect->Y += DiffY;
                RedrawMap = 2;
              }
              if(DraggingResize && CurLevelRect) {
                if(ResizeUp) { // BUG: if you drag the top edge of the rectangle past the top side of the map, the editor crashes
                  CurLevelRect->Y += DiffY;
                  CurLevelRect->H -= DiffY;
                  if(CurLevelRect->Y < 0)
                    CurLevelRect->Y = 0;
                }
                if(ResizeLeft) {
                  CurLevelRect->X += DiffX;
                  CurLevelRect->W -= DiffX;
                }
                if(ResizeDown)
                  CurLevelRect->H += DiffY;
                if(ResizeRight)
                  CurLevelRect->W += DiffX;
                if(CurLevelRect->W < 1) CurLevelRect->W = 1;
                if(CurLevelRect->H < 1) CurLevelRect->H = 1;
                RedrawMap = 2;
              }
              AvailableRect = LayerInfos[CurLayer].Map[(NewY+CameraY)*LayerInfos[CurLayer].LayerWidth+(NewX+CameraX)].Rect;
              if(AvailableRect == CurLevelRect)
                AvailableRect = NULL;

              CursorX = NewX;
              CursorY = NewY;
              if(LayerInfos[CurLayer].Type == LAYER_TILEMAP && SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
                GUIClick(SDL_BUTTON_RIGHT);
            }
            if(LayerInfos[CurLayer].Type != LAYER_TILEMAP && LayerInfos[CurLayer].Map[NewY*LayerInfos[CurLayer].LayerWidth+NewX].Rect)
              GUI_SetCursor(SYSCURSOR_HAND);
            if(CurLevelRect && !DraggingResize) {
              if(UpdateResizeDirections()) {
                if((ResizeUp || ResizeDown) && !(ResizeLeft||ResizeRight))
                  GUI_SetCursor(SYSCURSOR_SIZE_NS);
                if((ResizeLeft || ResizeRight) && !(ResizeUp||ResizeDown))
                  GUI_SetCursor(SYSCURSOR_SIZE_EW);
                if((ResizeUp && ResizeLeft) || (ResizeDown && ResizeRight))
                  GUI_SetCursor(SYSCURSOR_SIZE_NWSE);
                if((ResizeUp && ResizeRight) || (ResizeDown && ResizeLeft))
                  GUI_SetCursor(SYSCURSOR_SIZE_NESW);
                if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                  DraggingResize = 1;
                  DraggingMove = 0;
                }
              }
            }
            CursorShown = 1;
          } else {
            if(CursorShown) Redraw = 1;
            CursorShown = 0;
          }
        } else {
          if(IsInsideRect(e.motion.x, e.motion.y, TPRect.x, TPRect.y, TPRect.w, TPRect.h)) {
            int NewX = (e.motion.x - TPRect.x) / TileW;
            int NewY = (e.motion.y - TPRect.y) / TileH;
            if(NewX != TPCursorX || NewY != TPCursorY || !TPCursorShown) {
              Redraw = 1;
              TPCursorX = NewX;
              TPCursorY = NewY;
            }
            TPCursorShown = 1;
          } else {
            if(TPCursorShown)
              Redraw = 1;
            TPCursorShown = 0;
          }
        }
        if(!SetACursor)
          GUI_SetCursor(SYSCURSOR_NORMAL);
      } else if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) { 
          case SDLK_s: // Save
            if(e.key.keysym.mod & KMOD_CTRL)
              SaveLevel();
            break;
          case SDLK_x: // eXport
            if(e.key.keysym.mod & KMOD_CTRL)
              Sq_Export(); 
            break;
          case SDLK_b: // Bitmap
            if(e.key.keysym.mod & KMOD_CTRL)
              ExportBitmap();
            break;
          case SDLK_INSERT:
            if(!CurLevelRect) {
              for(LevelRect *Rect = LayerInfos[CurLayer].Rects; Rect; Rect=Rect->Next)
                if(Rect->X >= (CursorX+CameraX))
                  Rect->X++;
              Redraw = 1;
              RedrawMap = 3;
            }
            break;
          case SDLK_DELETE:
            if(CurLevelRect) {
              if(LayerInfos[CurLayer].Rects == CurLevelRect)
                LayerInfos[CurLayer].Rects = CurLevelRect->Next;
              if(CurLevelRect->ExtraInfo)
                free(CurLevelRect->ExtraInfo);
              if(CurLevelRect->Prev)
                CurLevelRect->Prev->Next = CurLevelRect->Next;
              if(CurLevelRect->Next)
                CurLevelRect->Next->Prev = CurLevelRect->Prev;
              free(CurLevelRect);
              AvailableRect = NULL;
              CurLevelRect = NULL;
            } else {
              for(LevelRect *Rect = LayerInfos[CurLayer].Rects; Rect; Rect=Rect->Next)
                if(Rect->X >= (CursorX+CameraX))
                  Rect->X--;
            }
            RedrawMap = 2;
            Redraw = 1;
            break;
        }
      } else if(e.type == SDL_KEYUP) {
      } else if(e.type == SDL_MOUSEBUTTONDOWN) {
        GUIClick(e.button.button);
      } else if(e.type == SDL_MOUSEBUTTONUP) {
      } else if(e.type == SDL_TEXTINPUT) {
        int OldLayer = CurLayer;
        char *TempPtr;
        LevelRect *RectPtr1, *RectPtr2;
        switch(*e.text.text) {
          case '=':
            if(!CurLevelRect) break;
            if(CurLevelRect->Prev) {
              if(LayerInfos[CurLayer].Rects == CurLevelRect->Prev)
                LayerInfos[CurLayer].Rects = CurLevelRect;
              RectPtr1 = CurLevelRect;
              RectPtr2 = CurLevelRect->Prev;
              RectPtr1->Prev = RectPtr2->Prev;
              if(RectPtr1->Prev)
                RectPtr1->Prev->Next = RectPtr1;
              RectPtr2->Next = RectPtr1->Next;
              if(RectPtr2->Next)
                RectPtr2->Next->Prev = RectPtr2;
              RectPtr1->Next = RectPtr2;
              RectPtr2->Prev = RectPtr1;
              Redraw = 1; RedrawMap = 2;
            }
            break;
          case '-':
            if(!CurLevelRect) break;
            if(CurLevelRect->Next) {
              if(LayerInfos[CurLayer].Rects == CurLevelRect)
                LayerInfos[CurLayer].Rects = CurLevelRect->Next;
              RectPtr1 = CurLevelRect;
              RectPtr2 = CurLevelRect->Next;
              RectPtr1->Next = RectPtr2->Next;
              if(RectPtr1->Next)
                RectPtr1->Next->Prev = RectPtr1;
              RectPtr2->Prev = RectPtr1->Prev;
              if(RectPtr2->Prev)
                RectPtr2->Prev->Next = RectPtr2;
              RectPtr1->Prev = RectPtr2;
              RectPtr2->Next = RectPtr1;
              Redraw = 1; RedrawMap = 2;
            }
            break;
          case '1': CurLayer = 0; Redraw = 1; break;
          case '2': if(NumLayers >= 2) CurLayer = 1; Redraw = 1; break;
          case '3': if(NumLayers >= 3) CurLayer = 2; Redraw = 1; break;
          case '4': if(NumLayers >= 4) CurLayer = 3; Redraw = 1; break;
          case '5': if(NumLayers >= 5) CurLayer = 4; Redraw = 1; break;
          case '6': if(NumLayers >= 6) CurLayer = 5; Redraw = 1; break;
          case '7': if(NumLayers >= 7) CurLayer = 6; Redraw = 1; break;
          case '8': if(NumLayers >= 8) CurLayer = 7; Redraw = 1; break;
          case '9': if(NumLayers >= 9) CurLayer = 8; Redraw = 1; break;
          case '0': if(NumLayers >= 10) CurLayer = 9; Redraw = 1; break;
          case 'q': CurLayer = (CurLayer+1)%NumLayers; Redraw = 1; break;
          case 'Q': CurLayer--; if(CurLayer < 0) CurLayer = NumLayers-1; Redraw = 1; break;
          case 'a': CameraX--; Redraw = 1; RedrawMap = 1; break;
          case 's': CameraY++; Redraw = 1; RedrawMap = 1; break;
          case 'd': CameraX++; Redraw = 1; RedrawMap = 1; break;
          case 'w': CameraY--; Redraw = 1; RedrawMap = 1; break;
          case 'A': CameraX-=10; Redraw = 1; RedrawMap = 1; break;
          case 'S': CameraY+=10; Redraw = 1; RedrawMap = 1; break;
          case 'D': CameraX+=10; Redraw = 1; RedrawMap = 1; break;
          case 'W': CameraY-=10; Redraw = 1; RedrawMap = 1; break;
          case 'j': if(CurLevelRect && CurLevelRect->X > 0) {CurLevelRect->X--; Redraw = 1; RedrawMap = 2;} break;
          case 'l': if(CurLevelRect && CurLevelRect->X < LayerInfos[CurLayer].LayerWidth-1) {CurLevelRect->X++; Redraw = 1; RedrawMap = 2;} break;
          case 'i': if(CurLevelRect && CurLevelRect->Y > 0) {CurLevelRect->Y--; Redraw = 1; RedrawMap = 2;} break;
          case 'k': if(CurLevelRect && CurLevelRect->Y < LayerInfos[CurLayer].LayerHeight-1) {CurLevelRect->Y++; Redraw = 1; RedrawMap = 2;} break;

          case 'J': if(CurLevelRect && CurLevelRect->W > 1) {CurLevelRect->W--; Redraw = 1; RedrawMap = 2;} break;
          case 'L': if(CurLevelRect) {CurLevelRect->W++; Redraw = 1; RedrawMap = 2;} break;
          case 'I': if(CurLevelRect && CurLevelRect->H > 1) {CurLevelRect->H--; Redraw = 1; RedrawMap = 2;} break;
          case 'K': if(CurLevelRect) {CurLevelRect->H++; Redraw = 1; RedrawMap = 2;} break;

          case 'G': GridOn ^= 1; Redraw = 1; RedrawMap = 2; break;
          case 'x': if(CurLevelRect) {CurLevelRect->Flips ^= SDL_FLIP_HORIZONTAL; Redraw = 1; RedrawMap = 2;} break;
          case 'y': if(CurLevelRect) {CurLevelRect->Flips ^= SDL_FLIP_VERTICAL; Redraw = 1; RedrawMap = 2;} break;
          case 'Y': if(CurLevelRect) {CurLevelRect->Flips = 0; Redraw = 1; RedrawMap = 2;} break;
          case 'e': if(TileOptions) break; TilePicker ^= 1; Redraw = 1; JustToggledTP = 1; break;
          case 'r': if(TilePicker) break; TileOptions ^= 1; Redraw = 1; JustToggledOptions = 1; break;
          case 'f':
            if(CurLevelRect) {
              *Temp = 0;
              if(CurLevelRect->ExtraInfo) {
                strcpy(Temp, CurLevelRect->ExtraInfo);
                free(CurLevelRect->ExtraInfo);
                CurLevelRect->ExtraInfo = NULL;
              }
              InputLine("Rectangle extra info", Temp, sizeof(Temp));
              if(*Temp)
                CurLevelRect->ExtraInfo = strdup(Temp);
              Redraw = 1;
            }
            break;
          default:
            TempPtr = strchr(ShiftNumbers, *e.text.text);
            if(TempPtr) {
              n = TempPtr - ShiftNumbers;
              if(n < NumLayers) {
                LayerInfos[n].LayerHidden ^= 1;
                Redraw = 1;
                RedrawMap = 1;
              }
            }
        }
        if(OldLayer != CurLayer) {
          CurLevelRect = NULL;
          TmpLevelRect.Flips = 0;
        }
      } else if(e.type == SDL_WINDOWEVENT) {
        switch(e.window.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            ScreenWidth = e.window.data1;
            ScreenHeight = e.window.data2;
          case SDL_WINDOWEVENT_EXPOSED:
            Redraw = 1;
        }
      }
      if(Redraw) {
        if(CurLevelRect) { // fix rectangle if it's out of bounds
          if(CurLevelRect->Y < 0)
            CurLevelRect->Y = 0;
          else if(CurLevelRect->Y >= LayerInfos[CurLayer].LayerHeight)
            CurLevelRect->Y = LayerInfos[CurLayer].LayerHeight - 1;
          if(CurLevelRect->X < 0)
            CurLevelRect->X = 0;
          else if(CurLevelRect->X >= LayerInfos[CurLayer].LayerWidth)
            CurLevelRect->X = LayerInfos[CurLayer].LayerWidth - 1;
        }
        if(RedrawMap == 2)
          RenderLevelRects(CurLayer);
        else if(RedrawMap == 3)
          for(int i=0; i<NumLayers; i++)
            RenderLevelRects(i);
        TPTexture  = LayerInfos[CurLayer].Texture;
        SDL_QueryTexture(TPTexture, NULL, NULL, &TPW, &TPH);
        TPRect.x = ScreenWidth/2-TPW/2;
        TPRect.y = ScreenHeight/2-TPH/2;
        TPRect.w = TPW;
        TPRect.h = TPH;
        if(JustToggledTP) {
          if(TilePicker)
            SDL_WarpMouseInWindow(window, TPRect.x+TPCursorX*TileW+TileW/2, TPRect.y+TPCursorY*TileH+TileH/2);
          else
            SDL_WarpMouseInWindow(window, MapViewX+CursorX*TileW+TileW/2, MapViewY+CursorY*TileH+TileH/2);
          JustToggledTP = 0;
        }

        MapViewWidth = (ScreenWidth - 30) / TileW;
        MapViewHeight = (ScreenHeight - 80) / TileH;
        if(MapViewWidth > LevelW) MapViewWidth = LevelW;
        if(MapViewHeight > LevelH) MapViewHeight = LevelH;
        MapViewWidthP = MapViewWidth*TileW;
        MapViewHeightP = MapViewHeight*TileH;
        MapViewX = (ScreenWidth / 2) - (MapViewWidthP / 2);
        MapViewY = ((ScreenHeight - 80) / 2) - (MapViewHeightP / 2) + 80;

        if(CameraX < 0) CameraX = 0;
        if(CameraY < 0) CameraY = 0;
        if((CameraX + MapViewWidth) > LevelW) CameraX = LevelW - MapViewWidth;
        if((CameraY + MapViewHeight) > LevelH) CameraY = LevelH - MapViewHeight;
        SDL_SetRenderDrawColor(ScreenRenderer, BGColor.r, BGColor.g, BGColor.b, 255);
        SDL_RenderClear(ScreenRenderer);

        char Temp[256];
        sprintf(Temp, "Layer %i (%s)", CurLayer+1, LayerInfos[CurLayer].Name);
        RenderSimpleText(ScreenRenderer, &MainFont, 5, 5+MainFont.Height*0, 0, Temp);
        if(CurLevelRect) {
          char Temp2[10] = "";
          if(CurLevelRect->W + CurLevelRect->H != 2)
            sprintf(Temp2, "<%ix%i> ", CurLevelRect->W, CurLevelRect->H);
          sprintf(Temp, "%s %s%s", LayerInfos[CurLayer].TilesetLookup[CurLevelRect->Type].Name, Temp2, CurLevelRect->ExtraInfo?CurLevelRect->ExtraInfo:"");
          int Graphic = LayerInfos[CurLayer].TilesetLookup[CurLevelRect->Type].Id;
          blitf(LayerInfos[CurLayer].Texture, ScreenRenderer, (Graphic&255)*TileW, (Graphic>>8)*TileH, 5, 5+MainFont.Height*1, TileW, TileH, CurLevelRect->Flips);
          RenderSimpleText(ScreenRenderer, &MainFont, 5+TileW+5, 5+MainFont.Height*1, 0, Temp);
        }
        RenderSimpleText(ScreenRenderer, &MainFont, 5, 5+MainFont.Height*2, 0, "1-9 change layers, wasd: scroll, ijkl: move, e: insert");

        SDL_SetRenderDrawColor(ScreenRenderer, FGColor.r, FGColor.g, FGColor.b, 255);
        SDL_RenderDrawLine(ScreenRenderer, 0, 60, ScreenWidth, 60);

        RedrawMap = 1;
        if(RedrawMap) {
          for(int L=0;L<NumLayers;L++)
            if(!LayerInfos[L].LayerHidden)
              for(int y=0;y<MapViewHeight;y++)
                for(int x=0;x<MapViewWidth;x++) {
                  int RealX = x+CameraX, RealY = y+CameraY;
                  LevelTile Tile = LayerInfos[L].Map[RealY*LayerInfos[L].LayerWidth+RealX];
                  if(Tile.Graphic != -1) {
                    if(Tile.Flips == 0)
                      blit(LayerInfos[L].Texture, ScreenRenderer, (Tile.Graphic&255)*TileW, (Tile.Graphic>>8)*TileH, MapViewX+x*TileW, MapViewY+y*TileH, TileW, TileH);
                    else
                      blitf(LayerInfos[L].Texture, ScreenRenderer, (Tile.Graphic&255)*TileW, (Tile.Graphic>>8)*TileH, MapViewX+x*TileW, MapViewY+y*TileH, TileW, TileH, Tile.Flips);
                  }
                }
          RedrawMap = 0;
        }

        for(int n=0;n<MapViewWidth;n++) {
          if(!((n+CameraX)&1)) {
            sprintf(Temp, "%.3X", n+CameraX);
            RenderSimpleText(ScreenRenderer, &TinyFont, MapViewX+n*TileW, MapViewY-16, 0, Temp);
            SDL_RenderDrawLine(ScreenRenderer, MapViewX+n*TileW+TileW/2, MapViewY, MapViewX+n*TileW+TileW/2, MapViewY-4);
          } else {
//            RenderSimpleText(ScreenRenderer, &TinyFont, MapViewX+n*TileW, MapViewY-28, 0, Label);
            SDL_RenderDrawLine(ScreenRenderer, MapViewX+n*TileW+TileW/2, MapViewY, MapViewX+n*TileW+TileW/2, MapViewY-16);
          }
        }
        for(int n=0;n<MapViewHeight;n++) {
          sprintf(Temp, "%.2X", n+CameraY);
          RenderSimpleText(ScreenRenderer, &TinyFont, MapViewX-15, MapViewY+n*TileH+TileH/2-TinyFont.Height/2, 0, Temp);          
          //SDL_RenderDrawLine(ScreenRenderer, MapViewX-15, MapViewY+n*TileH+TileH/2, MapViewX, MapViewY+n*TileH+TileH/2);
        }

        if(CursorShown) {
          SDL_SetRenderDrawColor(ScreenRenderer, SelectColor.r, SelectColor.g, SelectColor.b, 255);
          SDL_Rect Select = {MapViewX+CursorX*TileW, MapViewY+CursorY*TileH, TileW, TileH};
          SDL_SetRenderDrawBlendMode(ScreenRenderer, SDL_BLENDMODE_MOD);
          SDL_RenderFillRect(ScreenRenderer, &Select);
          SDL_SetRenderDrawBlendMode(ScreenRenderer, SDL_BLENDMODE_NONE);
          SDL_RenderDrawRect(ScreenRenderer, &Select);
        }

        if(CurLevelRect && CurLevelRect != &TmpLevelRect && LayerInfos[CurLayer].Type != LAYER_TILEMAP) {
          SDL_SetRenderDrawColor(ScreenRenderer, SelectColor.r, SelectColor.g, SelectColor.b, 255);
          SDL_Rect Select = MakeSelectRect(CurLevelRect, 8);
          SDL_RenderDrawRect(ScreenRenderer, &Select);

          if(LayerInfos[CurLayer].Type == LAYER_SPRITE) {
            if(CurLevelRect->Flips == SDL_FLIP_HORIZONTAL)
              SDL_RenderDrawLine(ScreenRenderer, Select.x-1, Select.y+Select.h/2, Select.x-10, Select.y+Select.h/2);
            else
              SDL_RenderDrawLine(ScreenRenderer, Select.x+Select.w, Select.y+Select.h/2, Select.x+Select.w+11, Select.y+Select.h/2);
          }
        }
        if(AvailableRect) {
          SDL_SetRenderDrawColor(ScreenRenderer, AvailableColor.r, AvailableColor.g, AvailableColor.b, 255);
          SDL_Rect Select = MakeSelectRect(AvailableRect, 4);
          SDL_RenderDrawRect(ScreenRenderer, &Select);
        }

        if(GridW && GridOn) {
          SDL_Rect Clip = {MapViewX, MapViewY, MapViewWidthP, MapViewHeightP};
          SDL_RenderSetClipRect(ScreenRenderer, &Clip);
          SDL_SetRenderDrawColor(ScreenRenderer, GridColor.r, GridColor.g, GridColor.b, 255);
          for(int i=0; i*GridW<(MapViewWidth+GridW); i++)
            for(int j=0; j*GridH<(MapViewHeight+GridH); j++) {
              SDL_Rect GridRect = {MapViewX+(i*GridW-CameraX%GridW)*TileW, MapViewY+(j*GridH-CameraY%GridH)*TileH, GridW*TileW, GridH*TileH};
              SDL_RenderDrawRect(ScreenRenderer, &GridRect);
            }
          SDL_RenderSetClipRect(ScreenRenderer, NULL);
        }

        SDL_SetRenderDrawColor(ScreenRenderer, FGColor.r, FGColor.g, FGColor.b, 255);
        rect(ScreenRenderer, MapViewX, MapViewY, MapViewX+MapViewWidthP, MapViewY+MapViewHeightP);
        if(TilePicker) {
          SDL_SetRenderDrawColor(ScreenRenderer, 255, 255, 255, 255);
          SDL_RenderFillRect(ScreenRenderer, &TPRect);
          SDL_RenderCopy(ScreenRenderer, TPTexture, NULL, &TPRect);

          // draw an outline
          SDL_Rect Outline = {TPRect.x-1, TPRect.y-1, TPRect.w+2, TPRect.h+2};
          SDL_SetRenderDrawColor(ScreenRenderer, FGColor.r, FGColor.g, FGColor.b, 255);
          SDL_RenderDrawRect(ScreenRenderer, &Outline);

          if(TPCursorShown) {
            SDL_SetRenderDrawColor(ScreenRenderer, SelectColor.r, SelectColor.g, SelectColor.b, 255);
            SDL_Rect Select = {TPRect.x+TPCursorX*TileW, TPRect.y+TPCursorY*TileH, TileW, TileH};
            SDL_SetRenderDrawBlendMode(ScreenRenderer, SDL_BLENDMODE_MOD);
            SDL_RenderFillRect(ScreenRenderer, &Select);
            SDL_SetRenderDrawBlendMode(ScreenRenderer, SDL_BLENDMODE_NONE);
            SDL_RenderDrawRect(ScreenRenderer, &Select);
          }
        }
        SDL_RenderPresent(ScreenRenderer);

        Redraw = 0;
      }
    }
    SDL_Delay(17);
  }
  for(int i=0;i<SYSCURSOR_MAX;i++)
    SDL_FreeCursor(SystemCursors[i]);
  Free_FontSet(&MainFont);
}
