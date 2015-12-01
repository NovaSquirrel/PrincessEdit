PrincessEdit
============

PrincessEdit is a generic tile-based level editor that is intended for game development on platforms where levels need to be compressed.
Levels are composed of a series of layers that can either consist of a tilemap or a series of rectangles. Rectangles can have additional information associated with them and can be horizontally and vertically flipped.
Levels are exported to the format the game would actually use through Squirrel scripts, and one is included for [Princess Engine](http://wiki.novasquirrel.com/Princess%20Engine) levels. Scripts later on will be usable for adding commands to the editor.

Dependencies
--------------
- [SDL2](https://www.libsdl.org/), plus [image](https://www.libsdl.org/projects/SDL_image/) and [ttf](https://www.libsdl.org/projects/SDL_ttf/)
- [Squirrel](http://www.squirrel-lang.org/)
- cJSON, included in the repository
