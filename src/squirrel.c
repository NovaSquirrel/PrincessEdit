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

int IsExporting = 0;
char ExportPath[260];

#define TOKENIZE_MULTI_WORD  1   /* "a b c" is a single word */
int XChatTokenize(const char *Input, char *WordBuff, const char **Word, const char **WordEol, int WordSize, int Flags) {
  int i;
  for(i=0;i<WordSize;i++) {
    Word[i] = "";
    WordEol[i] = "";
  }
  strcpy(WordBuff, Input);

  i = 0;
  char *Peek = WordBuff;
  while(1) {
    int IsMulti = Flags & TOKENIZE_MULTI_WORD && Peek[0] == '\"';
    char *Next;
    if(IsMulti) // todo: "" inside multiword tokens yet
      Next = strchr(Peek+1, '"');
    else
      Next = strchr(Peek+1, ' ');

    Word[i] = Peek+IsMulti;
    WordEol[i] = Input + (Peek - WordBuff);

    if(++i >= WordSize)
      break;

    if(!Next) break;
    *Next = 0;

    Peek = Next+1;
    while(*Peek == ' ') Peek++;
  }
  return i;
}

void Sq_PrintFunc(HSQUIRRELVM v,const SQChar *s,...) {
  va_list vl;
  va_start(vl, s);
  SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, s, vl);
  va_end(vl);
}

void Sq_ErrorFunc(HSQUIRRELVM v,const SQChar *s,...) {
  va_list vl;
  va_start(vl, s);
  SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, s, vl);
  va_end(vl);
}

SQInteger Sq_MessageBox(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "script message", What, NULL);
  return 0;
}

int FindLayerByName(const char *Name) {
  for(int i=0; i<NumLayers; i++)
    if(!strcasecmp(LayerInfos[i].Name, Name))
      return i;
  return -1;
}

SQInteger Sq_AskForInput(HSQUIRRELVM v) {
  return 0;
}

SQInteger Sq_GetLevelJSON(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);
  return 0;
}

SQInteger Sq_SetLevelJSON(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);
  return 0;
}

SQInteger Sq_Interpolate(HSQUIRRELVM v) {
  const SQChar *Str, *WordString;
  char Out[4096];
  sq_getstring(v, 2, &Str);
  sq_getstring(v, 3, &WordString);
  char *Poke = Out;
  const char *Peek = Str;

  char WordBuff[strlen(Str)+1];
  const char *Word[32];
  const char *WordEol[32];
  XChatTokenize(WordString, WordBuff, Word, WordEol, 32, TOKENIZE_MULTI_WORD);

  while(*Peek) {
    int Span = strcspn(Peek, "%&");
    memcpy(Poke, Peek, Span);
    Poke += Span;
    Peek += Span;
    if(*Peek == '%' || *Peek == '&') {
      char Extra = Peek[1];
      if(Extra == '%')
        *(Poke++) = '%';
      else if(Extra == '&')
        *(Poke++) = '&';
      else {
        if(isdigit(Extra)) {
          int WhichWord = Extra - '1';
          strcpy(Poke, (*Peek=='%')?Word[WhichWord]:WordEol[WhichWord]);
          Poke = strrchr(Poke, 0);
        } else { // look in the list of extra words
          int top = sq_gettop(v);;
          sq_pushnull(v); //null iterator
          while(SQ_SUCCEEDED(sq_next(v,-2))) {
             const SQChar *ThisWord;
             sq_getstring(v, -1, &ThisWord);
             if(ThisWord[0]==Extra) {
               strcpy(Poke, ThisWord+1);
               Poke = strrchr(Poke, 0);
               break;
             }
             sq_pop(v,2);
          }
          sq_pop(v,1); //pops the null iterator
          sq_settop(v, top);
        }
      }
      Peek+= 2;
    }
  }
  *Poke = 0;

  sq_pushstring(v, Out, -1);  
  return 1;
}

void Sq_DecodeJSONTable(HSQUIRRELVM v, cJSON *Item);

SQInteger Sq_GetLayerRects(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);
  int Layer = FindLayerByName(What);
  if(Layer < 0) {
    sq_pushnull(v);
    return 1;
  }

  sq_newarray(v, 0);
  for(LevelRect *Rect = LayerInfos[Layer].Rects; Rect; Rect=Rect->Next) {
    sq_newarray(v, 0);
    sq_pushstring(v, LayerInfos[Layer].TilesetLookup[Rect->Type].Name, -1);
    sq_arrayappend(v, -2);
    sq_pushinteger(v, Rect->X);
    sq_arrayappend(v, -2);
    sq_pushinteger(v, Rect->Y);
    sq_arrayappend(v, -2);
    sq_pushinteger(v, Rect->W);
    sq_arrayappend(v, -2);
    sq_pushinteger(v, Rect->H);
    sq_arrayappend(v, -2);
    sq_pushinteger(v, ((Rect->Flips&SDL_FLIP_HORIZONTAL)!=0)|(((Rect->Flips&SDL_FLIP_VERTICAL)!=0)<<1));
    sq_arrayappend(v, -2);
    sq_newtable(v); // layer, try to parse the ExtraInfo
    sq_arrayappend(v, -2);
    sq_arrayappend(v, -2);
  }

  return 1;
}

SQInteger Sq_SetLayerRects(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);
  int Layer = FindLayerByName(What);
  if(Layer < 0) {
    sq_pushnull(v);
    return 1;
  }

  return 0;
}

SQInteger Sq_GetLayerMap(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);
  int Layer = FindLayerByName(What);
  if(Layer < 0) {
    sq_pushnull(v);
    return 1;
  }
  RenderLevelRects(Layer);
  int ArgCount = sq_gettop(v);
  int X = 0, Y = 0, W = LayerInfos[Layer].LayerWidth, H = LayerInfos[Layer].LayerHeight;
  if(ArgCount == 5) {
    sq_getinteger(v, 3, &X);
    sq_getinteger(v, 4, &Y);
    sq_getinteger(v, 5, &W);
    sq_getinteger(v, 6, &H);
    W = abs(W);
    H = abs(H);
    if(X+W > LayerInfos[Layer].LayerWidth)
      W = LayerInfos[Layer].LayerWidth - X;
    if(Y+H > LayerInfos[Layer].LayerHeight)
      H = LayerInfos[Layer].LayerHeight - Y;
  }

  // FIX ME, dumps all the numbers in the first row for some reason
  sq_newarray(v, 0);
  while(H --> 0) {
     Y++;
     sq_newarray(v, 0);
     int OldX = X;
     while(W --> 0) {
       sq_pushinteger(v, TilesetLookupIdToIndex(Layer, LayerInfos[Layer].Map[Y*LayerInfos[Layer].LayerWidth+X++].Graphic));
       sq_arrayappend(v, -2);
     }
     sq_arrayappend(v, -2);
     X = OldX;
  }

  return 1;
}

SQInteger Sq_SetLayerMap(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);
  int Layer = FindLayerByName(What);
  if(Layer < 0) {
    sq_pushnull(v);
    return 1;
  }
//  int ArgCount = sq_gettop(v);

  return 0;
}

SQInteger Sq_RegisterExport(HSQUIRRELVM v) {
  const SQChar *What;
  HSQOBJECT Handler;
  sq_resetobject(&Handler);
  sq_getstackobj(v, 2, &Handler);
  sq_getstring(v, 3, &What);
  sq_addref(v, &Handler);

  SquirrelHook *NewHook = (SquirrelHook*)malloc(sizeof(SquirrelHook));
  if(!NewHook) exit(0);

  // add new export format
  strlcpy(NewHook->Name, What, sizeof(NewHook->Name));
  NewHook->Script = v;
  NewHook->Function = Handler;
  NewHook->Prev = NULL;
  NewHook->Next = AllExportFormats;
  if(AllExportFormats)
    AllExportFormats->Prev = NewHook;
  AllExportFormats = NewHook;
  return 0;
}

SQInteger Sq_RegisterMenu(HSQUIRRELVM v) {
  const SQChar *What;
  HSQOBJECT Handler;
  sq_resetobject(&Handler);
  sq_getstackobj(v, 2, &Handler);
  sq_getstring(v, 3, &What);
  sq_addref(v, &Handler);

  SquirrelHook *NewHook = (SquirrelHook*)malloc(sizeof(SquirrelHook));
  if(!NewHook) exit(0);

  // add new menu command
  strlcpy(NewHook->Name, What, sizeof(NewHook->Name));
  NewHook->Script = v;
  NewHook->Function = Handler;
  NewHook->Prev = NULL;
  NewHook->Next = AllMenuCommands;
  if(AllMenuCommands)
    AllMenuCommands->Prev = NewHook;
  AllMenuCommands = NewHook;
  return 0;
}

#define NUM_EXPORT_FILES 10
FILE *ExportFiles[NUM_EXPORT_FILES] = {NULL};

SQInteger Sq_ExportOpen(HSQUIRRELVM v) {
  if(!IsExporting)
    goto Error;

  const SQChar *What;
  sq_getstring(v, 2, &What);
  char Temp[260];

  // disallow too long file paths
  if((strlen(What)+strlen(ExportPath)+1) > sizeof(Temp)) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "ExportOpen filename too long");
    goto Error;
  }

  // disallow writing files outside of the directory the file is in
  if(!PathIsSafe(What)) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "ExportOpen filename attempting to write outside the file's directory");
    goto Error;
  }

  // try to open a file if there's space for one
  for(int i=0; i < NUM_EXPORT_FILES; i++)
    if(!ExportFiles[i]) {
      sprintf(Temp, "%s%s", ExportPath, What);
      FILE *File = fopen(Temp, "wb");
      if(!File) {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "ExportOpen can't open %s", Temp);
        goto Error;
      }
      ExportFiles[i] = File;
      sq_pushinteger(v, i);
      return 1;
    }

Error:
  sq_pushinteger(v, -1);
  return 1;
}

SQInteger Sq_ExportWrite(HSQUIRRELVM v) {
  if(!IsExporting)
    return 0;
  const SQChar *What;
  SQInteger Filenum;
  sq_getinteger(v, 2, &Filenum);
  sq_getstring(v, 3, &What);

  if(Filenum < 0 || Filenum >= NUM_EXPORT_FILES || !ExportFiles[Filenum])
    return 0;
  fprintf(ExportFiles[Filenum], "%s\r\n", What);
  return 0;
}

SQInteger Sq_ExportClose(HSQUIRRELVM v) {
  if(!IsExporting)
    return 0;
  SQInteger Filenum;
  sq_getinteger(v, 2, &Filenum);

  if(Filenum < 0 || Filenum >= NUM_EXPORT_FILES || !ExportFiles[Filenum])
    return 0;
  fclose(ExportFiles[Filenum]);
  ExportFiles[Filenum] = NULL;
  return 0;
}

SQInteger Sq_StrToL(HSQUIRRELVM v) {
  SQInteger Base;
  const SQChar *Str;
  sq_getstring(v, 2, &Str);
  sq_getinteger(v, 3, &Base);
  sq_pushinteger(v, strtol(Str, NULL, Base));
  return 1;
}

SQInteger Sq_GetInfo(HSQUIRRELVM v) {
  const SQChar *What;
  sq_getstring(v, 2, &What);

  if(!strcasecmp(What, "filename"))
    sq_pushstring(v, FilenameOnly(LevelFilename), -1);
  else if(!strcasecmp(What, "numlayers"))
    sq_pushinteger(v, NumLayers);
  else if(!strcasecmp(What, "width"))
    sq_pushinteger(v, LevelW);
  else if(!strcasecmp(What, "height"))
    sq_pushinteger(v, LevelH);
  else if(!strcasecmp(What, "cameraX"))
    sq_pushinteger(v, CameraX);
  else if(!strcasecmp(What, "cameraY"))
    sq_pushinteger(v, CameraY);
  else
    sq_pushnull(v);
  return 1;
}

SQInteger Sq_GetLevelInt(HSQUIRRELVM v) {
  const SQChar *Search;
  SQInteger Default;
  sq_getstring(v, 2, &Search);
  sq_getinteger(v, 3, &Default);
  char Search2[strlen(Search)+1];
  strcpy(Search2, Search);
  cJSON *Find = cJSON_Search(LevelJSON, Search2);
  if(Find) sq_pushinteger(v, Find->valueint);
  else sq_pushinteger(v, Default);
  return 1;
}

SQInteger Sq_GetLevelStr(HSQUIRRELVM v) {
  const SQChar *Search, *Default;
  sq_getstring(v, 2, &Search);
  sq_getstring(v, 3, &Default);
  char Search2[strlen(Search)+1];
  strcpy(Search2, Search);
  cJSON *Find = cJSON_Search(LevelJSON, Search2);
  if(Find) sq_pushstring(v, Find->valuestring, -1);
  else sq_pushstring(v, Default, -1);
  return 1;
}

SQInteger Sq_GetLevelTbl(HSQUIRRELVM v) {
  const SQChar *Search;
  sq_getstring(v, 2, &Search);
  char Search2[strlen(Search)+1];
  strcpy(Search2, Search);
  cJSON *Find = cJSON_Search(LevelJSON, Search2);
  if(Find) {
    sq_newtable(v);
    Sq_DecodeJSONTable(v, Find->child);
  }
  else sq_pushnull(v);
  return 1;
}

void Sq_DecodeJSONTable(HSQUIRRELVM v, cJSON *Item) {
  if(!Item)
    return;
  while(Item) {
    if(Item->string)
      sq_pushstring(v, Item->string, -1);
    switch(Item->type) {
      case cJSON_False:
        sq_pushbool(v, SQFalse);
        break;
      case cJSON_True:
        sq_pushbool(v, SQTrue);
        break;
      case cJSON_NULL:
        sq_pushnull(v);
        break;
      case cJSON_Number:
        if(Item->valueint == Item->valuedouble)
          sq_pushinteger(v, Item->valueint);
        else
          sq_pushfloat(v, Item->valuedouble);
        break;
      case cJSON_String:
        sq_pushstring(v, Item->valuestring, -1);
        break;
      case cJSON_Array:
        sq_newarray(v, 0);
        Sq_DecodeJSONTable(v, Item->child);
        break;
      case cJSON_Object:
        sq_newtable(v);
        Sq_DecodeJSONTable(v, Item->child);
        break;
    }
    if(Item->string)
      sq_newslot(v,-3,SQFalse);
    else
      sq_arrayappend(v, -2);
    Item = Item->next;
  }
}

SQInteger Sq_DecodeJSON(HSQUIRRELVM v) {
  const SQChar *Str;
  sq_getstring(v, 2, &Str);
  if(Str[0]!='{' && Str[0]!='[') {
    if(!strcmp(Str, "true"))
      sq_pushbool(v, SQTrue);
    else if(!strcmp(Str, "false"))
      sq_pushbool(v, SQFalse);
    else if(isdigit(Str[0]) || (Str[0]=='-' && isdigit(Str[1])))
      sq_pushinteger(v, strtol(Str, NULL, 0));
    else
      sq_pushstring(v, Str, -1);
    return 1;
  }

  cJSON *Root = cJSON_Parse(Str);
  if(!Root || !Root->child) {
    sq_pushnull(v);
    return 1;
  }
  sq_newtable(v);
  Sq_DecodeJSONTable(v, Root->child);
  cJSON_Delete(Root);
  return 1;
}

int Sq_DoFile(HSQUIRRELVM VM, const char *File) {
  return sqstd_dofile(VM, _SC(File), SQFalse, SQTrue);
}

void Sq_RegisterFunc(HSQUIRRELVM v,SQFUNCTION f,const char *fname, int ParamNum, const char *params) {
  sq_pushstring(v,fname,-1);
  sq_newclosure(v,f,0);
  sq_newslot(v,-3,SQFalse);
  if(params)
    sq_setparamscheck(v, ParamNum, _SC(params)); 
}

HSQUIRRELVM Sq_Open(const char *File) {
  HSQUIRRELVM v; 
  v = sq_open(1024);
  sqstd_seterrorhandlers(v);
  sq_setprintfunc(v, Sq_PrintFunc,Sq_ErrorFunc);

  sq_pushroottable(v);
  sqstd_register_mathlib(v);
  sqstd_register_stringlib(v);
  sq_pop(v,1);

  sq_pushroottable(v);
  sq_pushstring(v, "api", -1);
  sq_newtable(v);
  sq_newslot(v,-3,SQFalse);
  sq_pushstring(v, "api", -1);
  sq_get(v, -2);

  Sq_RegisterFunc(v,   Sq_StrToL,         "Num",            2, "si");
  Sq_RegisterFunc(v,   Sq_MessageBox,     "MessageBox",     1, "s");
  Sq_RegisterFunc(v,   Sq_AskForInput,    "AskForInput",    1, "s");
  Sq_RegisterFunc(v,   Sq_GetLevelJSON,   "GetLevelJSON",   1, "si.");
  Sq_RegisterFunc(v,   Sq_SetLevelJSON,   "SetLevelJSON",   2, "s.");
  Sq_RegisterFunc(v,   Sq_GetLayerRects,  "GetLayerRects",  2, "s");
  Sq_RegisterFunc(v,   Sq_SetLayerRects,  "SetLayerRects",  2, "sa");
  Sq_RegisterFunc(v,   Sq_GetLayerMap,    "GetLayerMap",   -1, "siiii");
  Sq_RegisterFunc(v,   Sq_SetLayerMap,    "SetLayerMap",   -2, "saiiii");
  Sq_RegisterFunc(v,   Sq_RegisterExport, "RegisterExport", 2, "cs");
  Sq_RegisterFunc(v,   Sq_RegisterMenu,   "RegisterMenu",   2, "cs");
  Sq_RegisterFunc(v,   Sq_ExportOpen,     "ExportOpen",     1, "s");
  Sq_RegisterFunc(v,   Sq_ExportWrite,    "ExportWrite",    2, "is");
  Sq_RegisterFunc(v,   Sq_ExportClose,    "ExportClose",    1, "i");
  Sq_RegisterFunc(v,   Sq_GetInfo,        "GetInfo",        1, "s");
  Sq_RegisterFunc(v,   Sq_GetLevelInt,    "GetLevelInt",    2, "is");
  Sq_RegisterFunc(v,   Sq_GetLevelStr,    "GetLevelStr",    2, "ss");
  Sq_RegisterFunc(v,   Sq_GetLevelTbl,    "GetLevelTbl",    1, "s");
  Sq_RegisterFunc(v,   Sq_DecodeJSON,     "DecodeJSON",     1, "s");
  Sq_RegisterFunc(v,   Sq_Interpolate,    "Interpolate",    3, "ssa");
  sq_pop(v,1); // pop api table

  Sq_DoFile(v, "data/common.nut");

  if(File) Sq_DoFile(v, File);
  return v;
}

void Sq_Close(HSQUIRRELVM v) {
// recipe for disaster: Sq_Close and sq_close with different behavior
  sq_pop(v,1);
  sq_close(v); 
}

SquirrelScript *FindAddon(const char *Name, SquirrelScript **First) {
  for(SquirrelScript *Find = *First; Find; Find=Find->Next)
    if(!strcasecmp(Name, Find->Name))
      return Find;
  return NULL;
}

void UnloadAddon(SquirrelScript *Addon);

SquirrelScript *LoadAddon(const char *Path) {
  SquirrelScript *NewAddon = NULL;

  // check if the addon already exists
  const char *ScriptName = FilenameOnly(Path);

  if(!AllScripts) {
    NewAddon = (SquirrelScript*)calloc(1,sizeof(SquirrelScript));
    AllScripts = NewAddon;
  } else {
    SquirrelScript *Find = AllScripts;
    while(Find->Next) {
      if(!strcasecmp(ScriptName, Find->Name)) // don't load same addon more than once
        return NULL;
      Find = Find->Next;
    }
    NewAddon = (SquirrelScript*)calloc(1,sizeof(SquirrelScript)); // all pointers NULL
    Find->Next = NewAddon;
    NewAddon->Prev = Find;
  }
  if(!NewAddon)
    return NULL;

  NewAddon->Script = Sq_Open(Path);
  strlcpy(NewAddon->Name, ScriptName, sizeof(NewAddon->Name));
  sq_setforeignptr(NewAddon->Script, (SQUserPointer)NewAddon);
  return NewAddon;
}

void UnloadAddon(SquirrelScript *Addon) {
  if(!Addon) return;
  if(Addon == AllScripts) AllScripts = Addon->Next;
  // to do: undo the hooks the plugin made
  if(Addon->Script) Sq_Close(Addon->Script);
  free(Addon);
  return;
}

int Sq_Export() {
  // take the level path and take off the filename
  strcpy(ExportPath, LevelFilenameFull);
  char *Temp = strrchr(ExportPath, '/');
  if(!Temp)
    Temp = strrchr(ExportPath, '\\');
  if(!Temp)
   strcpy(ExportPath, "./");
  if(Temp)
    Temp[1] = 0;
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Exporting to \"%s\"", ExportPath);

  const char *ExportFormat = GetLevelStr("", "Meta/ExportFormat");
  if(!ExportFormat || !*ExportFormat) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "No export format specified in the file");
    return 0; // no format
  }

  SquirrelHook *Hook = AllExportFormats;
  for(; Hook; Hook=Hook->Next) {
    if(!strcasecmp(Hook->Name, ExportFormat))
      break;
  }
  if(!Hook) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "No exporter registered for \"%s\"", ExportFormat);
    return 0; // format not found
  }

  IsExporting = 1;
  SQInteger top = sq_gettop(Hook->Script);
  sq_pushroottable(Hook->Script);
  sq_pushobject(Hook->Script, Hook->Function);
  sq_pushroottable(Hook->Script); // push "this"
  sq_call(Hook->Script, 1, SQTrue, SQTrue);
  IsExporting = 0;

  // if we return a true, then it was successful
  SQBool BoolResult = SQFalse;
  if(OT_BOOL == sq_gettype(Hook->Script,-1))
    sq_getbool(Hook->Script, -1, &BoolResult);
  sq_settop(Hook->Script,top);

  return (BoolResult==SQTrue)?1:0;
}
