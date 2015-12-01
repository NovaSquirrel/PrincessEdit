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

int quit = 0;
char *PrefPath = NULL;
char *BasePath = NULL;
void StartGUI();
SquirrelScript *AllScripts = NULL;
SquirrelHook *AllMenuCommands = NULL;
SquirrelHook *AllExportFormats = NULL;

int main( int argc, char* args[] ) {
  if(argc != 2) {
     SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Open what level?");
     return 0;
  }

  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 0;
  }
  SDL_SetHint("SDL_HINT_VIDEO_ALLOW_SCREENSAVER", "1");

  PrefPath = SDL_GetPrefPath("Bushytail Software","PrincessEdit");
  BasePath = SDL_GetBasePath();

  FILE *Test = fopen(args[1], "rb");
  if(Test) {
    fclose(Test);
    if(!LoadLevel(args[1])) {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Can't load %s", args[1]);
      return 0;
    }
  } else {
    if(!LoadLevel("data/default.json")) {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Can't load default level");
      return 0;
    }
    strlcpy(LevelFilename, FilenameOnly(args[1]), sizeof(LevelFilename));
    strlcpy(LevelFilenameFull, args[1], sizeof(LevelFilenameFull));
  }

  StartGUI();

  if(PrefPath) SDL_free(PrefPath);
  if(BasePath) SDL_free(BasePath);
  SDL_Quit();

  while(AllScripts)
    UnloadAddon(AllScripts);
  return 0;
}
