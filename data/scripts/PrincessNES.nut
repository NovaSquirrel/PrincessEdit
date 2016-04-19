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
  {"T":"BRICKS",         "W":256, "H":16, "O": "LObjN LO::RECT_1,           &X, &Y, &H, LN1::BRICKS, &W"},
  {"T":"SOLID_BLOCK",    "W":1,   "H":1,  "O": "LObj  LO::S_SOLID_BLOCK,    &X, &Y"},
  {"T":"SOLID_BLOCK",    "W":16,  "H":16, "O": "LObjN LO::R_SOLID_BLOCK,    &X, &Y, &W, &H"},
  {"T":"SOLID_BLOCK",    "W":256, "H":16, "O": "LObjN LO::RECT_1,           &X, &Y, &H, LN1::SOLID_BLOCK, &W"},
  {"T":"BRICKPRIZE",     "W":1,   "H":1,  "O": "LObj  LO::S_BRICKPRIZE,     &X, &Y"},
  {"T":"BRICKPRIZE",     "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::BRICKPRIZE"},
  {"T":"PRIZE",          "W":1,   "H":1,  "O": "LObj  LO::S_PRIZE,          &X, &Y"},
  {"T":"PRIZE",          "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::PRIZE"},
  {"T":"USED_PRIZE",     "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::USED_PRIZE"},
  {"T":"USED_PRIZE",     "W":1,   "H":16, "O": "LObjN LO::TALL_1,           &X, &Y, &W, LN1::USED_PRIZE"},
  {"T":"COIN",           "W":1,   "H":1,  "O": "LObj  LO::S_COIN,           &X, &Y"},
  {"T":"COIN",           "W":16,  "H":16, "O": "LObjN LO::R_COIN,           &X, &Y, &W, &H"},
  {"T":"SPRING",         "W":1,   "H":1,  "O": "LObj  LO::S_SPRING,         &X, &Y"},
  {"T":"ROCK_GROUND",    "W":16,  "H":16, "O": "LObjN LO::R_ROCK,           &X, &Y, &W, &H"},
  {"T":"ROCK_GROUND",    "W":256, "H":16, "O": "LObjN LO::RECT_1,           &X, &Y, &H, LN1::ROCK, &W"},
  {"T":"SMALL_ROCK",     "W":1,   "H":1,  "O": "LObj  LO::S_SOLID_ROCK,     &X, &Y"},
  {"T":"SOLID_LEDGE",    "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::SOLID_LEDGE"},
  {"T":"SOLID_LEDGE",    "W":256, "H":1,  "O": "LObjN LO::RECT_1,           &X, &Y, &W, LN1::SOLID_LEDGE, &W"},
  {"T":"FALLTHROUGH_LEDGE", "W":16, "H":1,"O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::FALLTHROUGH_LEDGE"},
  {"T":"FALLTHROUGH_LEDGE", "W":256, "H":1,"O": "LObjN LO::RECT_1,           &X, &Y, &W, LN1::FALLTHROUGH_LEDGE, &W"},
  {"T":"SIGNPOST",       "W":1,   "H":1,  "O": "LObj  LO::S_SIGNPOST,       &X, &Y"},
  {"T":"WATER",          "W":16,  "H":16, "O": "LObjN LO::R_WATER,          &X, &Y, &W, &H"},
  {"T":"WATER",          "W":256, "H":16, "O": "LObjN LO::RECT_1,           &X, &Y, &H, LN1::WATER, &W"},
  {"T":"LADDER",         "W":1,   "H":16, "O": "LObjN LO::TALL_1,           &X, &Y, &H, LN1::LADDER"},
  {"T":"SPIKES",         "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::SPIKES"},
  {"T":"DOOR",           "W":1,   "H":1,  "O": "LObj  LO::S_DOOR,           &X, &Y"},
  {"T":"EXIT_DOOR",      "W":1,   "H":1,  "O": "LObj  LO::S_EXIT_DOOR,      &X, &Y"},
  {"T":"BG_TALL_GRASS",  "W":1,   "H":1,  "O": "LObj  LO::S_TALL_GRASS,     &X, &Y"},
  {"T":"BG_FLOWER_1",    "W":1,   "H":1,  "O": "LObj  LO::S_FLOWER,         &X, &Y"},
  {"T":"BG_BUSH_BOT",    "W":1,   "H":1,  "O": "LObj  LO::S_BUSH,           &X, &Y"},
  {"T":"BG_FENCE",       "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::FENCE"},
  {"T":"BG_TALL_GRASS",  "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::TALLGRASS"},
  {"T":"BG_FLOWER_1",    "W":16,  "H":1,  "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::FLOWER"},
  {"T":"BG_BLACK",       "W":16,  "H":16, "O": "LObjN LO::RECT_2,           &X, &Y, &H, LN2::BLACK, &W"},
  {"T":"BG_TRUNK",       "W":1,   "H":16, "O": "LObjN LO::TALL_2,           &X, &Y, &H, LN2::TRUNK"},
  {"T":"BG_TRUNK_L",     "W":1,   "H":16, "O": "LObjN LO::TALL_2,           &X, &Y, &H, LN2::TRUNK_L"},
  {"T":"BG_BUSH_BOT",    "W":16,  "H":1,  "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::BUSH"},
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

function PrincessExport() {
  // rects: [Type, X, Y, W, H, Flips, Extra]
  const RTYPE = 0;  const RX = 1;
  const RY = 2;     const RW = 3;
  const RH = 4;     const RFLIPS = 5;
  const REXTRA = 6;

  local FG = api.GetLayerRects("Foreground");
  local SP = api.GetLayerRects("Sprites");
  local CT = api.GetLayerRects("Control");
  local Header = api.GetLevelTbl("Header");
  if(SP.len() > 84) {
    print("Too many sprites! You have "+(SP.len()-84)+" too many.");
    return false;
  }

  local Width = api.GetInfo("width");
  local Height = api.GetInfo("height");

  // Find boundaries
  // a b c d e f g h i j k l m n o L
  // 0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F
  local Boundaries = 0;
  function SetBoundaryAt(Screen) {
    if(Screen < 1) return;
    if(Screen > 16) return;
    local Bit = 1 << 16-Screen;
    Boundaries = Boundaries | Bit;
  }

  foreach(R in CT)
    if(R[RTYPE]=="SCROLL_LOCK") {
      local Screen = R[RX]/16;
      if((R[RX]&15) == 15)
        SetBoundaryAt(Screen+1);
      else
        SetBoundaryAt(Screen);
    }

  local HScreens = Width/16;
  local VScreens = Height/15;
  local UseLinks = VScreens > 1;

  print("Height "+Height)
  print("VScreens "+VScreens)

  // Rearrange level into a big horizontal strip
  if(VScreens > 1) {
    foreach(List in [FG, SP, CT])
      for(local i=0; i<List.len(); i++) {
        local R = List[i];
        local V = R[RY] / 15;
        List[i][RX] += V*Width;
        List[i][RY] = List[i][RY] % 15;
      }
  }

  // find player start position
  local StartX = 0;
  local StartY = 0;
  local FacingLeft = 0;
  local PlayerPos = FindType(CT, ["PLAYER_START_L", "PLAYER_START_R"]);
  if(PlayerPos.len()) {
    StartX = PlayerPos[0][RX];
    StartY = PlayerPos[0][RY];
    if(PlayerPos[0][RTYPE] == "PLAYER_START_L")
      FacingLeft = 64;
  }

  // sort layers by X position
  // later: actually take Z ordering into consideration for the FG layer
  // for now, Y position is used
  function SortRects(A, B) {
    local test1 = (A[RX]<<8)|A[RY];
    local test2 = (B[RX]<<8)|B[RY];
    return test1 <=> test2;
  }
  FG.sort(SortRects);
  SP.sort(SortRects);
  CT.sort(SortRects);

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
  api.ExportWrite(File, "  .byt "+Hex((((Width/16)-1)<<4) | (StartY-1)));

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

  // write boundaries
  if(UseLinks)
    Boundaries = Boundaries | 1;
  api.ExportWrite(File, format("  .byt $%.2x, $%.2x ; boundaries", Boundaries>>8, Boundaries&255));

  // write links if used
  if(UseLinks) {
    for(local i=0; i<VScreens; i++) {
      local Up = ((-HScreens)&15)<<4;
      local Down = HScreens;
      if(i==0) Up = 0;
      if(i==VScreens-1) Down = 0;
      api.ExportWrite(File, format("  .byt $%.2x, $%.2x ; link", (HScreens-1)&15, Up|Down));
    }
    if(HScreens*VScreens != 16)
      api.ExportWrite(File, format("  .byt $%.2x, $00", (16-(HScreens*VScreens)-1)&15));  
  }

  // do the data section
  api.ExportWrite(File, "");
  api.ExportWrite(File, Filename+"Data:");

  // write all the lines for the level FG
  local LastX = 0;
  foreach(R in FG) {
    local Rule = FindRuleFor(R[RTYPE], R[RW], R[RH]);

    local XDifference = R[RX] - LastX;
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
        api.ExportWrite(File, "  LSetX "+ R[RX]);
        XDifference = 0;
      }
    }
    LastX = R[RX];

    api.ExportWrite(File, "  "+api.Interpolate(RectRules[Rule].O, "", ["X"+XDifference, "Y"+R[RY], "W"+(R[RW]-1), "H"+(R[RH]-1), "T"+R[RTYPE]]));
  }
  api.ExportWrite(File, "  LFinished");

  // do the sprite section
  api.ExportWrite(File, "");
  api.ExportWrite(File, Filename+"Sprite:");
  foreach(sprite in SP) {
    api.ExportWrite(File, format("  LSpr Enemy::%-20s %i, %3i, %3i", sprite[0]+",", (sprite[RFLIPS]&1), sprite[RX], sprite[RY]));
  }
  api.ExportWrite(File, "  .byt 255 ; end");

  api.ExportClose(File);
  api.MessageBox("Exported successfully");
  return true;
}

api.RegisterExport(PrincessExport, "PrincessNES");
