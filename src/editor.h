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
#ifndef PRINCESS_HEADER
#define PRINCESS_HEADER
#define NO_STDIO_REDIRECT
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <stdarg.h>
#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdstring.h>
#include <sqstdmath.h>
#include <sqstdaux.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#define SHARED_OBJ_EXTENSION "dll"
#else
#define SHARED_OBJ_EXTENSION "so"
#endif
#include "cJSON.h"

enum FontVersion {
  FONT_NORMAL,
  FONT_BOLD,
  FONT_ITALIC,
  FONT_BOLD_ITALIC,
};

//////////////////// editor-specific stuff ///////////////////
typedef struct LevelRect {
  short Type, X, Y, W, H;
  char Flips;
  char *ExtraInfo; // stored as a series of JSON values; "a":1, "b":2, etc. maybe a cJSON list instead?
  struct LevelRect *Prev, *Next;
} LevelRect;

typedef struct LevelTile {
  short Graphic;          // -1 if no tile
  LevelRect *Rect;        // or NULL if not Rectangle mode
  signed short Angle;
  char Flips;         // SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL
} LevelTile;

typedef struct TilesetEntry { // convert to some sort of hash table later
  int Id;                     // 32 bytes total
  char Name[28];
} TilesetEntry;

enum LayerTypes {
  LAYER_RECTANGLE,
  LAYER_SPRITE,
  LAYER_TILEMAP,
  LAYER_CONTROL
};

typedef struct LayerInfo {
  SDL_Texture *Texture; // tileset texture
  int TextureW, TextureH;
  int Type;
  char LayerHidden;
  char Name[32];
  char TilesetName[32];
  TilesetEntry *TilesetLookup;
  unsigned int TileWidth, TileHeight, LayerWidth, LayerHeight;
  LevelTile *Map;
  LevelRect *Rects;
  cJSON *JSON;
} LayerInfo;

///////////////////////// Squirrel ///////////////////////////
typedef struct SquirrelScript {
  char Name[32];
  HSQUIRRELVM Script;
  struct SquirrelScript *Prev, *Next;
} SquirrelScript;

typedef struct SquirrelHook {
  char Name[50];
  HSQUIRRELVM Script;
  HSQOBJECT Function;
  struct SquirrelHook *Prev, *Next;
} SquirrelHook;

//////////////////////////////////////////////////////////////

typedef struct FontSet {
  TTF_Font *Font[4];
  int Width, Height;
} FontSet;

extern FontSet MainFont;

// dialog stuff
struct GUIDialog;
struct GUIState;
typedef int (*DIALOG_PROC)(int msg, struct GUIDialog *d, struct GUIState *s);

typedef struct GUIDialog {
   DIALOG_PROC proc;
   int x, y, w, h;               /* position and size of the object */
   int fg, bg;                   /* foreground and background colors */
   int flags;                    /* flags about the object state */
   int d1, d2;                   /* any data the object might require */
   void *dp, *dp2, *dp3;         /* pointers to more object data */
   union {
     void *Window;
     void *dp4;
   } Extra;
} GUIDialog;

typedef struct GUIState {
  int Flags, X,Y, WinWidth, WinHeight;
  SDL_Renderer *Renderer;
  void (*UpdateFunc)(struct GUIState *s);
  FontSet *DefaultFont;
  GUIDialog *Dialog;
  SDL_Window *Window;

  GUIDialog *PriorityClickDialog;
  GUIDialog *DragDialog;
  const char *MessageText; // text sent to a widget, like for a key press
  union {
    SDL_Keysym *SDL; // key sent to a widget 
    int Curses;
  } MessageKey;
} GUIState;

enum GSFFlags {
  GSF_NEED_INITIALIZE = 1, /* needs initialize messages sent */
  GSF_NEED_REDRAW = 2,     /* needs draw messages sent */
  GSF_NEED_PRESENT = 4,    /* needs SDL_RenderPresent() */
  GSF_NEED_UPDATE = 8,     /* needs updated to fit the window */
};

struct TextEditInfo {
  int   ScrollChars,  CursorChars,  SelectChars;
  char *ScrollBytes, *CursorBytes, *SelectBytes;
  int   HasSelect;
};
#define MAX_MENU_LEVELS 5
typedef struct MenuState {
  int CurLevel;   // current menu level
  int NumOptions; // number of options for the current menu item
  int X, Y, CurHilight;
  unsigned int Ticks;
  cJSON *Menus[MAX_MENU_LEVELS];
  const char *Picked[MAX_MENU_LEVELS];
  char Include[70];
  char *Target;
} MenuState;

enum SysCursorId {
  SYSCURSOR_NORMAL,
  SYSCURSOR_WAIT,
  SYSCURSOR_SIZE_NS,
  SYSCURSOR_SIZE_EW,
  SYSCURSOR_SIZE_NWSE,
  SYSCURSOR_SIZE_NESW,
  SYSCURSOR_SIZE_ALL,
  SYSCURSOR_HAND,
  SYSCURSOR_DISABLE,
  SYSCURSOR_MAX
};
extern SDL_Cursor *SystemCursors[SYSCURSOR_MAX];

/* bits for the flags field */
#define D_EXIT          1        /* object makes the dialog exit */
#define D_SELECTED      2        /* object is selected */
#define D_GOTFOCUS      4        /* object has the input focus */
#define D_GOTMOUSE      8        /* mouse is on top of object (unused) */
#define D_HIDDEN        16       /* object is not visible */
#define D_DISABLED      32       /* object is visible but inactive */
#define D_DIRTY         64       /* object needs to be redrawn */
#define D_KEYWATCH      128      /* object wants MSG_KEY even without focus */
#define D_WANTCLICKAWAY 256      /* object will get MSG_CLICKAWAY when clicking on something else */

/* return values for the dialog procedures */
#define D_O_K           0        /* normal exit status */
#define D_CLOSE         1        /* request to close the dialog */
#define D_REDRAW        2        /* request to redraw the dialog */
#define D_REDRAWME      4        /* request to redraw this object */
#define D_WANTFOCUS     8        /* this object wants the input focus */
#define D_CAN_DRAG      16       /* click can be extended into a drag */
#define D_REDRAW_ALL    32       /* request to redraw all active dialogs */
#define D_DONTWANTMOUSE 64       /* this object does not want mouse focus */

/* messages for the dialog procedures */
#define MSG_START       1        /* start the dialog, initialise */
#define MSG_END         2        /* dialog is finished - cleanup */
#define MSG_DRAW        3        /* draw the object */
#define MSG_CLICK       4        /* mouse click on the object */
#define MSG_DCLICK      5        /* double click on the object */
#define MSG_KEY         6        /* key press, read MessageKey */
#define MSG_CHAR        7        /* text input, read MessageText */
#define MSG_DRAG        8        /* object is being dragged */
#define MSG_DRAGDROPPED 9        /* cursor dropped after dragging */
#define MSG_WANTFOCUS   10       /* does object want the input focus? */
#define MSG_GOTFOCUS    11       /* got the input focus (not needed) */
#define MSG_LOSTFOCUS   12       /* lost the input focus */
#define MSG_GOTMOUSE    13       /* mouse on top of object */
#define MSG_LOSTMOUSE   14       /* mouse moved away from object */
#define MSG_IDLE        15       /* update any background stuff */
#define MSG_RADIO       16       /* clear radio buttons */
#define MSG_WHEEL       17       /* mouse wheel moved */
#define MSG_LPRESS      18       /* mouse left button pressed */
#define MSG_LRELEASE    19       /* mouse left button released */
#define MSG_MPRESS      20       /* mouse middle button pressed */
#define MSG_MRELEASE    21       /* mouse middle button released */
#define MSG_RPRESS      22       /* mouse right button pressed */
#define MSG_RRELEASE    23       /* mouse right button released */
#define MSG_WANTMOUSE   24       /* does object want the mouse? */

#define MSG_ACTIVATE    25       /* activate menus and such */
#define MSG_CLICKAWAY   26       /* clicked on something else */
#define MSG_MOUSEMOVE   27       /* given to the PriorityClickDialog */
#define MSG_WIN_RESIZE  28       /* window was resized */

#define MSG_INFO_ADDED  29       /* info was added */

FILE *fopen_with_basepath(const char *Path, const char *Modes);
void strlcpy(char *Destination, const char *Source, int MaxLength);
int memcasecmp(const char *Text1, const char *Text2, int Length);
void rectfill(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2);
void rect(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2);
void sblit(SDL_Surface* SrcBmp, SDL_Surface* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height);
void blit(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height);
void blitf(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, SDL_RendererFlip Flip);
void blitz(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Width2, int Height2);
void blitfull(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int DestX, int DestY);
void SDL_MessageBox(int Type, const char *Title, SDL_Window *Window, const char *fmt, ...);
SDL_Surface *SDL_LoadImage(const char *FileName, int Flags);
SDL_Texture *LoadTexture(const char *FileName, int Flags);
void Free_FontSet(FontSet *Fonts);
int RenderSimpleText(SDL_Renderer *Renderer, FontSet *Font, int Flags, int X, int Y, const char *Text);
int RenderRotateText(SDL_Renderer *Renderer, FontSet *Font, int X, int Y, int Flags, double Rotation, const char *Text);
int Load_FontSet(FontSet *Fonts, int Size, const char *Font2, const char *Font1, const char *Font3, const char *Font4);
const char *FilenameOnly(const char *Path);
cJSON *cJSON_Search(cJSON *Root, char *SearchString);
const char *GetLevelStr(const char *Default, const char *s,...);
int GetLevelInt(int Default, const char *s,...);
cJSON *cJSON_Load(const char *Filename);
int cJSON_Length(cJSON *Array);
int cJSON_IntValue(cJSON *JSON, const char *Var, int Default);
void cJSON_IntValueSet(cJSON *JSON, const char *Var, int Value);
int LoadLevel(const char *Filename);
void GUI_SetCursor(int CursorNum);
GUIDialog *FindDialogWithProc(const char *Context, GUIState *InState, GUIState **State, DIALOG_PROC Proc);
int IsInsideRect(int X1, int Y1, int X2, int Y2, int W, int H);
void TextInterpolate(char *Out, const char *In, char Prefix, const char *ReplaceThis, const char *ReplaceWith[]);
char *FindCloserPointer(char *A, char *B);
void LoadTilesets();
int TilesetLookupIdToIndex(int Layer, int Id);
int TilesetLookupStringToIndex(int Layer, const char *String);
char *ReadTextFile(const char *Filename);
void RenderLevelRects(int Layer);
void AutoloadDirectory(const char *Directory);
SquirrelScript *LoadAddon(const char *Path);
void UnloadAddon(SquirrelScript *Addon);
int Sq_Export();
void SaveLevel();
int PathIsSafe(const char *Path);
#define ZeroStruct(x) memset(&x, sizeof(x), 0);

// Squirrel
int Sq_DoFile(HSQUIRRELVM VM, const char *File);
HSQUIRRELVM Sq_Open(const char *File);
void Sq_Close(HSQUIRRELVM v);

// dialog widgets
int RunWidget(int msg,struct GUIDialog *d, GUIState *s);
int Widget_ScreenClear(int msg,struct GUIDialog *d, GUIState *s);
int Widget_StaticText(int msg,struct GUIDialog *d, GUIState *s);
int Widget_TextEdit(int msg,struct GUIDialog *d, GUIState *s);
int Widget_ChatEdit(int msg,struct GUIDialog *d, GUIState *s);
int Widget_StaticImage(int msg,struct GUIDialog *d, GUIState *s);
int Widget_MenuButton(int msg,struct GUIDialog *d, GUIState *s);
int Widget_PopupMenu(int msg,struct GUIDialog *d, GUIState *s);
int Widget_ChannelTabs(int msg, struct GUIDialog *d, GUIState *s);
int Widget_ChatView(int msg,struct GUIDialog *d, GUIState *s);
int Widget_Dummy(int msg, struct GUIDialog *d, GUIState *s);

extern SDL_Texture *ScreenTexture;
extern int quit;
extern char *PrefPath;
extern char *BasePath;
extern cJSON *LevelJSON;
extern int LevelW;
extern int LevelH;
extern int GridW;
extern int GridH;
extern int TileW;
extern int TileH;
extern int CameraX;
extern int CameraY;

extern SDL_Color FGColor;
extern SDL_Color BGColor;
extern SDL_Color SelectColor;
extern SDL_Color AvailableColor;
extern SDL_Color GridColor;
extern char LevelFilename[50];
extern char LevelFilenameFull[260];
extern LayerInfo *LayerInfos;
extern int NumLayers;
extern SquirrelScript *AllScripts;
extern SquirrelHook *AllMenuCommands;
extern SquirrelHook *AllExportFormats;

#endif
