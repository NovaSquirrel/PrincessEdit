/*
LWriteCol Col1, Col2, Col3
*/

local RectRules = [
  {"T":"EMPTY",          "W":16,  "H":16, "O": "LObjN LO::R_AIR,            &X, &Y, &W, &H"},
  {"T":"GROUND",         "W":1,   "H":1,  "O": "LObj  LO::S_GROUND,         &X, &Y"},
  {"T":"GROUND",         "W":16,  "H":16, "O": "LObjN LO::R_GROUND,         &X, &Y, &W, &H"},
  {"T":"GROUND",         "W":256, "H":16, "O": "LObjN LO::RECT_1,           &X, &Y, &H, LN1::GROUND, &W"},
  {"T":"GROUND_CLIMB_L", "W":1,   "H":1,  "O": "LObj  LO::S_GROUND_CLIMB_L, &X, &Y"},
  {"T":"GROUND_CLIMB_R", "W":1,   "H":1,  "O": "LObj  LO::S_GROUND_CLIMB_R, &X, &Y"},
  {"T":"BRICKS",         "W":1,   "H":1,  "O": "LObj  LO::S_BRICK,          &X, &Y"},
  {"T":"BRICKS",         "W":16,  "H":16, "O": "LObjN LO::R_BRICK,          &X, &Y, &W, &H"},
  {"T":"BRICKS",         "W":256, "H":16, "O": "LobjN LO::RECT_1,           &X, &Y, &H, LN1::BRICKS, &W"},
  {"T":"SOLID_BLOCK",    "W":1,   "H":1,  "O": "LObj  LO::S_SOLID_BLOCK,    &X, &Y"},
  {"T":"SOLID_BLOCK",    "W":16,  "H":16, "O": "LObjN LO::R_SOLID_BLOCK,    &X, &Y, &W, &H"},
  {"T":"SOLID_BLOCK",    "W":256, "H":16, "O": "LObjN LO::RECT_1,           &X, &Y, &H, LN1::SOLID_BLOCK, &W"},
  {"T":"PRIZE_BRICK",    "W":1,   "H":1,  "O": "LObj  LO::S_BRICKPRIZE,     &X, &Y"},
  {"T":"PRIZE",          "W":1,   "H":1,  "O": "LObj  LO::S_PRIZE,          &X, &Y"},
  {"T":"PRIZE",          "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::PRIZE"},
  {"T":"USED_PRIZE",     "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::USED_PRIZE"},
  {"T":"USED_PRIZE",     "W":1,   "H":16, "O": "LObjN LO::TALL_1,           &X, &Y, &W, LN1::USED_PRIZE"},
  {"T":"COIN",           "W":1,   "H":1,  "O": "LObj  LO::S_COIN,           &X, &Y"},
  {"T":"COIN",           "W":16,  "H":16, "O": "LObjN LO::R_COIN,           &X, &Y, &W, &H"},
  {"T":"SPRING",         "W":1,   "H":1,  "O": "LObj  LO::S_SPRING,         &X, &Y"},
  {"T":"ROCK_GROUND",    "W":16,  "H":16, "O": "LObjN LO::R_ROCK,           &X, &Y, &W, &H"},
  {"T":"ROCK_GROUND",    "W":256, "H":16, "O": "LobjN LO::RECT_1,           &X, &Y, &H, LN1::ROCK, &W"},
  {"T":"SMALL_ROCK",     "W":1,   "H":1,  "O": "LObj  LO::R_SOLID_ROCK,     &X, &Y"},
  {"T":"SOLID_LEDGE",    "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::SOLID_LEDGE"},
  {"T":"SOLID_LEDGE",    "W":256, "H":1,  "O": "LObjN LO::RECT_1,           &X, &Y, &W, LN1::SOLID_LEDGE, &W"},
  {"T":"FALLTHROUGH_LEDGE", "W":16, "H":1,"O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::FALLTHROUGH_LEDGE"},
  {"T":"FALLTHROUGH_LEDGE", "W":256, "H":1,"O": "LObjN LO::RECT_1,           &X, &Y, &W, LN1::FALLTHROUGH_LEDGE, &W"},
  {"T":"SIGNPOST",       "W":1,   "H":1,  "O": "LObj  LO::S_SIGNPOST,       &X, &Y"},
  {"T":"WATER",          "W":16,  "H":16, "O": "LobjN LO::R_WATER,          &X, &Y, &W, &H"},
  {"T":"WATER",          "W":256, "H":16, "O": "LobjN LO::RECT_1,           &X, &Y, &H, LN1::WATER, &W"},
  {"T":"LADDER",         "W":1,   "H":16, "O": "LObjN LO::TALL_1,           &X, &Y, &H, LN1::LADDER"},
  {"T":"SPIKES",         "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::SPIKES"},
  {"T":"DOOR",           "W":1,   "H":1,  "O": "LObj  LO::S_DOOR,           &X, &Y"},
  {"T":"EXIT_DOOR",      "W":1,   "H":1,  "O": "LObj  LO::S_EXIT_DOOR,      &X, &Y"},
  {"T":"",               "W":1,   "H":1,  "O": "LObj  LO::S_CUSTOM,         &X, &Y, Metatiles::&T"},
  {"T":"",               "W":16,  "H":16, "O": "LObj  LO::R_CUSTOM,         &X, &Y, Metatiles::&T, (&W<<4)|&H"},
];

function Hex(int) {
  return format("$%x", int);
}

function FindRuleFor(Type, W, H) {
  for(local i=0; i < RectRules.len(); i++) {
    if((Type == RectRules[i].T || !RectRules[i].T.len()) && RectRules[i].W >= W && RectRules[i].H >= H)
      return i;
  }
  return -1;
}

function FindType(Array, Types) {
  function Filter(Index, Value) {
    return Types.find(Value[0])!=null;
  }
  return Array.filter(Filter);
}

/* function TryRects(R) {
  local Res;
  Res = FindRuleFor(Type, R[3], R[4]);
  if(Res >= 0) return 0;
  Res = FindRuleFor(Type, 16, R[4]);
  if(Res >= 0) return 1;
  Res = FindRuleFor(Type, R[3], 1);
  if(Res >= 0) return 2;
  Res = FindRuleFor(Type, 1, R[4]);
  if(Res >= 0) return 3;
  print("Can't find any rules for "+R[0]);
  return -1;
} */

function PrincessExport() {
  // rects: [Type, X, Y, W, H, Flips, Extra]
  local FG = api.GetLayerRects("Foreground");
  local SP = api.GetLayerRects("Sprites");
  local CT = api.GetLayerRects("Control");
  local Header = api.GetLevelTbl("Header");
  if(SP.len() > 84) {
    print("Too many sprites! You have "+(SP.len()-84)+" too many.");
    return false;
  }

  local Width = api.GetInfo("width");
  // find player start position
  local StartX = 0;
  local StartY = 0;
  local FacingLeft = 0;
  local PlayerPos = FindType(CT, ["PLAYER_START_L", "PLAYER_START_R"]);
  if(PlayerPos.len()) {
    StartX = PlayerPos[0][1];
    StartY = PlayerPos[0][2];
    if(PlayerPos[0][0] == "PLAYER_START_L")
      FacingLeft = 64;
  }

  // sort layers by X position
  // later: actually take Z ordering into consideration for the FG layer
  FG.sort(@(a,b) a[1] <=> b[1]);
  SP.sort(@(a,b) a[1] <=> b[1]);
  CT.sort(@(a,b) a[1] <=> b[1]);

  // open the output file
  local Filename = split(api.GetInfo("filename"), ".")[0];
  local ExportName = Filename+".s";
  local File = api.ExportOpen(ExportName);
  if(File < 0) // file couldn't open?
    return false;

  // label for the header
  api.ExportWrite(File, Filename+":");

  // write music, start X, screens and Y position
  // ;ad.mmmmm
  // ;|| +++++- music
  // ;|+------- if 1, player starts facing left
  // ;+-------- if 1, use alternate buffer for collected bits

  api.ExportWrite(File, "  .byt MusicTracks::"+Header.Music+"|"+FacingLeft);
  api.ExportWrite(File, "  .byt "+StartX);
  api.ExportWrite(File, "  .byt "+Hex((((Width/16)-1)<<4) + StartY));

  // write sprite graphics slots
  for(local i=0; i<4; i++)
    api.ExportWrite(File, "  .byt GraphicsUpload::"+Header.SpriteGFX[i]);

  // write pointers to the foreground and sprite data
  api.ExportWrite(File, "  .addr "+Filename+"Data");
  api.ExportWrite(File, "  .addr "+Filename+"Sprite");
 
  // write background
  api.ExportWrite(File, "  .byt "+Hex(Header.BGColor)+" ; background");
 
  // write graphics upload list
  foreach(i in Header.GFXUpload)
    api.ExportWrite(File, "  .byt GraphicsUpload::"+i);
  api.ExportWrite(File, "  .byt 255 ; end");

  // to do: actually figure out the boundaries
  api.ExportWrite(File, "  .word %0000000000000000 ; boundaries");

  // do the data section
  api.ExportWrite(File, "");
  api.ExportWrite(File, Filename+"Data:");

  // write all the lines for the level FG
  local LastX = 0;
  foreach(R in FG) {
    local Rule = FindRuleFor(R[0], R[3], R[4]);

    local XDifference = R[1] - LastX;
    if(XDifference >= 16 || XDifference < 0) {
      if(XDifference == 16) {
        api.ExportWrite(File, "  LXPlus16");
        XDifference = 0;
      } else if(XDifference < 32) {
        api.ExportWrite(File, "  LXPlus16");
        XDifference -= 16;
      } else if(XDifference == -16) {
        api.ExportWrite(File, "  LXMinus16");
        XDifference = 0 ;
      } else {
        api.ExportWrite(File, "  LSetX "+ R[1]);
        XDifference = 0;
      }
    }
    LastX = R[1];

    api.ExportWrite(File, "  "+api.Interpolate(RectRules[Rule].O, "", ["X"+XDifference, "Y"+R[2], "W"+R[3], "H"+R[4], "T"+R[0]]));
  }
  api.ExportWrite(File, "  LFinished");

  // do the sprite section
  api.ExportWrite(File, "");
  api.ExportWrite(File, Filename+"Sprite:");
  foreach(sprite in SP) {
    api.ExportWrite(File, format("  LSpr Enemy::%-20s %i, %3i, %3i", sprite[0]+",", (sprite[5]&1), sprite[1], sprite[2]));
  }
  api.ExportWrite(File, "  .byt 255 ; end");

  api.ExportClose(File);
  return true;
}

api.RegisterExport(PrincessExport, "PrincessNES");
