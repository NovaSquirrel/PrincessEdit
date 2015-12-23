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

int IsInsideRect(int X1, int Y1, int X2, int Y2, int W, int H) {
  if(X1<X2) return 0;
  if(Y1<Y2) return 0;
  if(X1>=(X2+W)) return 0;
  if(Y1>=(Y2+H)) return 0;
  return 1;
}

void AutoloadDirectory(const char *Directory) {
  DIR *dir;
  struct dirent *ent;
  if((dir = opendir(Directory))) {
    while((ent = readdir(dir))) {
      if(*ent->d_name == '.') continue;
      char *Temp = strrchr(ent->d_name, '.');
      if(!Temp) continue;
      if(strcasecmp(Temp+1, "nut")) continue;
      char FullPath[strlen(Directory)+strlen(ent->d_name)+1];
      strcpy(FullPath, Directory);
      strcat(FullPath, ent->d_name);
      LoadAddon(FullPath);
    }
    closedir(dir);
  }
}

char *ReadTextFile(const char *Filename) {
  FILE *File = fopen(Filename, "rb");
  if(!File) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s not found", Filename);
    return NULL;
  }
  fseek(File, 0, SEEK_END);
  unsigned long FileSize = ftell(File);
  rewind(File);
  char *Buffer = (char*)malloc(sizeof(char)*(FileSize+1));
  if(!Buffer || (FileSize != fread(Buffer,1,FileSize,File))) {
    fclose(File);
    return NULL;
  }
  Buffer[FileSize] = 0;
  fclose(File);
  return Buffer;
}

FILE *fopen_with_basepath(const char *Path, const char *Modes) {
  char RealPath[strlen(Path)+strlen(BasePath)+1];
  sprintf(RealPath, "%s%s", Path, BasePath);
  return fopen(RealPath, Modes);
}

int cJSON_Length(cJSON *Array) {
  if(!Array)
    return 0;
  int out = 1;
  while(Array->next) {
    out++;
    Array = Array->next;
  }
  return out;
}

int cJSON_IntValue(cJSON *JSON, const char *Var, int Default) {
  cJSON *Target = cJSON_GetObjectItem(JSON, Var);
  if(!Target) return Default;
  if(Target->type == cJSON_Number) return Target->valueint;
  if(Target->type == cJSON_False) return 0;
  if(Target->type == cJSON_True) return 1;
  return Default;
}

void cJSON_IntValueSet(cJSON *JSON, const char *Var, int Value) {
  cJSON *Target = cJSON_GetObjectItem(JSON, Var);
  if(Target->type == cJSON_Number) {
    Target->valueint = Value;
    Target->valuedouble = Value;
  }
  if(Target->type == cJSON_False || Target->type == cJSON_True)
    Target->type = Value?cJSON_True:cJSON_False;
}

cJSON *cJSON_Load(const char *Filename) {
  char *Buffer = ReadTextFile(Filename);
  cJSON *JSON = cJSON_Parse(Buffer);
  free(Buffer);
  if(!JSON)
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s failed to load", Filename);
  return JSON;
}

cJSON *cJSON_Search(cJSON *Root, char *SearchString) {
// SearchString is formatted like "Path1\\Path2\\Path3". Individual paths are like a/b/c/d/e

  char *Paths = SearchString;
  do {
    char *ThisPath = Paths;
    Paths = strchr(ThisPath, '\\');
    if(Paths) *(Paths++) = 0;

    cJSON *Find = Root;
    while(1) {
      char *ThisItem = ThisPath;
      ThisPath = strchr(ThisItem, '/');
      if(ThisPath) *(ThisPath++) = 0;
      Find = cJSON_GetObjectItem(Find,ThisItem);
      if(!Find) // try another path then
        break;
      if(!ThisPath) // no more items?
        return Find; // this is the right item
    }
  } while (Paths);
  return NULL;
}

const char *GetLevelStr(const char *Default, const char *s,...) {
  char Path[512]; va_list vl; va_start(vl, s);
  vsprintf(Path, s, vl); va_end(vl);
  cJSON *Find = cJSON_Search(LevelJSON, Path);
  if(Find) return Find->valuestring;
  return Default;
}

int GetLevelInt(int Default, const char *s,...) {
  char Path[512]; va_list vl; va_start(vl, s);
  vsprintf(Path, s, vl); va_end(vl);
  cJSON *Find = cJSON_Search(LevelJSON, Path);
  if(Find) return Find->valueint;
  return Default;
}

void SDL_MessageBox(int Type, const char *Title, SDL_Window *Window, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  char Buffer[512];
  vsprintf(Buffer, fmt, argp);
  SDL_ShowSimpleMessageBox(Type, Title, Buffer, Window);
  va_end(argp);
}

int PathIsSafe(const char *Path) {
  if(strstr(Path, "..")) return 0; // no parent directories
  if(Path[0] == '/') return 0;     // no root
  if(strchr(Path, ':')) return 0;  // no drives
  return 1;
}

void TextInterpolate(char *Out, const char *In, char Prefix, const char *ReplaceThis, const char *ReplaceWith[]) {
  while(*In) {
    if(*In != Prefix)
      *(Out++) = *(In++);
    else {
      In++;
      char *Find = strchr(ReplaceThis, *(In++));
      if(Find) {
        int This = Find - ReplaceThis;
        strcpy(Out, ReplaceWith[This]);
        Out += strlen(ReplaceWith[This]);
      } else {
        *(Out++) = Prefix;
        *(Out++) = In[-1];
      }
    }
  }
  *Out = 0;
}

void strlcpy(char *Destination, const char *Source, int MaxLength) {
  // MaxLength is directly from sizeof() so it includes the zero
  int SourceLen = strlen(Source);
  if((SourceLen+1) < MaxLength)
    MaxLength = SourceLen + 1;
  memcpy(Destination, Source, MaxLength-1);
  Destination[MaxLength-1] = 0;
}

int memcasecmp(const char *Text1, const char *Text2, int Length) {
  for(;Length;Length--)
    if(tolower(*(Text1++)) != tolower(*(Text2++)))
      return 1;
  return 0;
}

const char *FilenameOnly(const char *Path) {
  const char *Temp = strrchr(Path, '/');
  if(!Temp) Temp = strrchr(Path, '\\');
  if(!Temp) return Path;
  return Temp+1;
}
