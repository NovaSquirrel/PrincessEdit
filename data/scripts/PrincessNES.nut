local ColDataPointerTypes = ["SIGNPOST", "STORY_DIALOG_TRIGGER"]; // list of object types whose extra data is a pointer
local PrizeContainingTypes = ["PRIZE", "BRICKPRIZE"]; // list of object types whose extra data is an inventory type
local FGControlTypes = ["COLUMN_DATA", "COLUMN_POINTER", "BLOCK_CONTENTS", "TELEPORT"];

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
  {"T":"ROCK_GROUND",    "W":256, "H":16, "O": "LObjN LO::RECT_3,           &X, &Y, &H, LN3::ROCK, &W"},
  {"T":"SMALL_ROCK",     "W":1,   "H":1,  "O": "LObj  LO::S_SOLID_ROCK,     &X, &Y"},
  {"T":"SMALL_ROCK",     "W":16,  "H":16, "O": "LObjN LO::R_SOLID_ROCK,     &X, &Y, &W, &H"},
  {"T":"SMALL_ROCK",     "W":256, "H":1,  "O": "LObjN LO::RECT_3,           &X, &Y, &W, LN3::SOLID_ROCK, &W"},
  {"T":"SOLID_LEDGE",    "W":16,  "H":1,  "O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::SOLID_LEDGE"},
  {"T":"SOLID_LEDGE",    "W":256, "H":1,  "O": "LObjN LO::RECT_1,           &X, &Y, &W, LN1::SOLID_LEDGE, &W"},
  {"T":"FALLTHROUGH_LEDGE", "W":16, "H":1,"O": "LObjN LO::WIDE_1,           &X, &Y, &W, LN1::FALLTHROUGH_LEDGE"},
  {"T":"FALLTHROUGH_LEDGE", "W":256,"H":1,"O": "LObjN LO::RECT_1,           &X, &Y, &W, LN1::FALLTHROUGH_LEDGE, &W"},
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
  {"T":"BG_BLACK",       "W":256, "H":16, "O": "LObjN LO::RECT_2,           &X, &Y, &H, LN2::BLACK, &W"},
  {"T":"BG_TRUNK",       "W":1,   "H":16, "O": "LObjN LO::TALL_2,           &X, &Y, &H, LN2::TRUNK"},
  {"T":"BG_TRUNK_L",     "W":1,   "H":16, "O": "LObjN LO::TALL_2,           &X, &Y, &H, LN2::TRUNK_L"},
  {"T":"BG_BUSH_BOT",    "W":16,  "H":1,  "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::BUSH"},
  {"T":"GRAY_BRICKS",    "W":16,  "H":1,  "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::GRAY_BRICKS"},
  {"T":"GRAY_BRICKS",    "W":1,   "H":16, "O": "LObjN LO::TALL_2,           &X, &Y, &H, LN2::GRAY_BRICKS"},
  {"T":"GRAY_BRICKS",    "W":16,  "H":16, "O": "LObjN LO::R_GRAYBRICKS,     &X, &Y, &W, &H"},
  {"T":"GRAY_BRICKS",    "W":256, "H":16, "O": "LObjN LO::RECT_2,           &X, &Y, &H, LN2::GRAY_BRICKS, &W"},
  {"T":"BIG_HEART",      "W":1,   "H":1,  "O": "LObj  LO::S_BIGHEART,       &X, &Y"},
  {"T":"HEART",          "W":1,   "H":1,  "O": "LObj  LO::S_HEART,          &X, &Y"},
  {"T":"STONE_BRIDGE",   "W":16,  "H":1,  "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::STONE_BRIDGE"},
  {"T":"PALM_TREE",      "W":1,   "H":16, "O": "LObjN LO::TALL_2,           &X, &Y, &H, LN2::PALM_TREE"},
  {"T":"SAND"            "W":16,  "H":16, "O": "LObjN LO::R_SAND,           &X, &Y, &W, &H"},
  {"T":"SAND"            "W":256, "H":16, "O": "LObjN LO::RECT_2,           &X, &Y, &H, LN2::SAND, &W"},
  {"T":"STRIPED_LOG_HORIZ", "W":16,"H":1, "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::STRIPED_LOG_HORIZ"},
  {"T":"MOAI_RIGHT",     "W":1,   "H":1,  "O": "LObj  LO::S_MOAI_RIGHT,     &X, &Y"},
  {"T":"MOAI_LEFT",      "W":1,   "H":1,  "O": "LObj  LO::S_MOAI_LEFT,      &X, &Y"},
  {"T":"TROPICAL_FLOWER",  "W":1, "H":1,  "O": "LObj  LO::S_TROPICAL_FLOWER,&X, &Y"},
  {"T":"TROPICAL_FLOWER_2","W":1, "H":1,  "O": "LObj  LO::S_TROPICAL_FLOWER_2, &X, &Y"},
  {"T":"LOG_HORIZ",      "W":16,  "H":1,  "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::LOG_HORIZ"},
  {"T":"BG_TREETOP_LL",  "W":1,   "H":1,  "O": "LObj  LO::S_BIG_BUSH, &X, &Y"},
  {"T":"BIG_SPIKY_BUSH", "W":1,   "H":1,  "O": "LObj  LO::S_BIG_SPIKY_BUSH, &X, &Y"},
  {"T":"WOOD_PLATFORM_TOP", "W":1, "H":1, "O": "LObj  LO::S_WOOD_PLATFORM_TOP, &X, &Y"},
  {"T":"WOOD_PLATFORM_TOP", "W":16,"H":1, "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::WOOD_PLATFORM_TOP"},
  {"T":"WOOD_PLATFORM",  "W":1,   "H":1,  "O": "LObj  LO::S_WOOD_PLATFORM,  &X, &Y"},
  {"T":"WOOD_PLATFORM",  "W":16,  "H":16, "O": "LObjN LO::R_WOOD_PLATFORM,  &X, &Y, &W, &H"},
  {"T":"PATH_LINE",      "W":16,  "H":1,  "O": "LObjN LO::WIDE_2,           &X, &Y, &W, LN2::PATH_LINE"},
  {"T":"PATH_LINE",      "W":1,   "H":16, "O": "LObjN LO::TALL_2,           &X, &Y, &H, LN2::PATH_LINE"},
  {"T":"SQUIRREL_BUSH",   "W":1,   "H":1, "O": "LObj  LO::S_SKWIRLBUSH,     &X, &Y"},
  {"T":"SQUIRREL_BUSH",   "W":16,  "H":16,"O": "LObjN LO::R_SKWIRLBUSH,     &X, &Y, &W, &H"},
  {"T":"K_STATUE_TOP",    "W":1,   "H":1, "O": "LObj  LO::S_STATUE,         &X, &Y"},
  {"T":"BRICKWALL_MIDDLE","W":16,  "H":16,"O": "LObjN LO::R_BRICKWALL,      &X, &Y, &W, &H"},
  {"T":"WHITEFENCE_MIDDLE","W":16,  "H":1,"O": "LObjN LO::WIDE_3,           &X, &Y, &W, LN3::WHITEFENCE"},
  {"T":"WOOD_ARROW_LEFT", "W":1,   "H":1, "O": "LObj  LO::S_W_ARROW_LEFT,   &X, &Y"},
  {"T":"WOOD_ARROW_DOWN", "W":1,   "H":1, "O": "LObj  LO::S_W_ARROW_DOWN,   &X, &Y"},
  {"T":"WOOD_ARROW_UP",   "W":1,   "H":1, "O": "LObj  LO::S_W_ARROW_UP,     &X, &Y"},
  {"T":"WOOD_ARROW_RIGHT","W":1,   "H":1, "O": "LObj  LO::S_W_ARROW_RIGHT,  &X, &Y"},
  {"T":"WOOD_BOMB",       "W":1,   "H":1, "O": "LObj  LO::S_W_BOMB,         &X, &Y"},
  {"T":"WOOD_CRATE",      "W":1,   "H":1, "O": "LObj  LO::S_W_CRATE,        &X, &Y"},
  {"T":"METAL_ARROW_LEFT", "W":1,  "H":1, "O": "LObj  LO::S_M_ARROW_LEFT,   &X, &Y"},
  {"T":"METAL_ARROW_DOWN", "W":1,  "H":1, "O": "LObj  LO::S_M_ARROW_DOWN,   &X, &Y"},
  {"T":"METAL_ARROW_UP",   "W":1,  "H":1, "O": "LObj  LO::S_M_ARROW_UP,     &X, &Y"},
  {"T":"METAL_ARROW_RIGHT","W":1,  "H":1, "O": "LObj  LO::S_M_ARROW_RIGHT,  &X, &Y"},
  {"T":"METAL_BOMB",       "W":1,  "H":1, "O": "LObj  LO::S_M_BOMB,         &X, &Y"},
  {"T":"METAL_CRATE",      "W":1,  "H":1, "O": "LObj  LO::S_M_CRATE,        &X, &Y"},
  {"T":"BG_GLASS_GREEN", "W":1,   "H":1,  "O": "LObj  LO::S_BG_GLASS_GREEN, &X, &Y"},
  {"T":"BG_GLASS_GRAY",  "W":1,   "H":1,  "O": "LObj  LO::S_BG_GLASS_GRAY,  &X, &Y"},
  {"T":"BG_GLASS_RED",   "W":1,   "H":1,  "O": "LObj  LO::S_BG_GLASS_RED,   &X, &Y"},
  {"T":"BG_GLASS_BLUE",  "W":1,   "H":1,  "O": "LObj  LO::S_BG_GLASS_BLUE,  &X, &Y"},
  {"T":"FG_GLASS_RED",   "W":1,   "H":1,  "O": "LObj  LO::S_FG_GLASS_RED,   &X, &Y"},
  {"T":"FG_GLASS_BLUE",  "W":1,   "H":1,  "O": "LObj  LO::S_FG_GLASS_BLUE,  &X, &Y"},
  {"T":"BG_GLASS_GREEN", "W":16,  "H":16, "O": "LObjN LO::R_BG_GLASS_GREEN, &X, &Y, &W, &H"},
  {"T":"BG_GLASS_GRAY",  "W":16,  "H":16, "O": "LObjN LO::R_BG_GLASS_GRAY,  &X, &Y, &W, &H"},
  {"T":"BG_GLASS_RED",   "W":16,  "H":16, "O": "LObjN LO::R_BG_GLASS_RED,   &X, &Y, &W, &H"},
  {"T":"BG_GLASS_BLUE",  "W":16,  "H":16, "O": "LObjN LO::R_BG_GLASS_BLUE,  &X, &Y, &W, &H"},
  {"T":"FG_GLASS_RED",   "W":16,  "H":16, "O": "LObjN LO::R_FG_GLASS_RED,   &X, &Y, &W, &H"},
  {"T":"FG_GLASS_BLUE",  "W":16,  "H":16, "O": "LObjN LO::R_FG_GLASS_BLUE,  &X, &Y, &W, &H"},
  {"T":"CEILING_BARRIER","W":1,   "H":1,  "O": "LObj  LO::S_CEILING_BARRIER,&X, &Y"},
  {"T":"CEILING_BARRIER","W":16,  "H":16, "O": "LObjN LO::R_CEILING_BARRIER,&X, &Y, &W, &H"},
  {"T":"STORY_DIALOG_TRIGGER", "W": 1, "H":1, "O": "LObj  LO::S_STORY_TRIGGER,         &X, &Y"},
  {"T":"GLIDER_BLOCK",   "W":1,   "H":1,  "O": "LObj  LO::S_GLIDER_BLOCK,   &X, &Y"},
  {"T":"MINE_TRACK",                    "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK,         &X, &Y"},
  {"T":"MINE_TRACK",                    "W":16,  "H":1,  "O": "LObjN LO::WIDE_3,           &X, &Y, &W, LN3::MINE_TRACKS"},
  {"T":"MINE_TRACK_BRAKES",             "W":16,  "H":1,  "O": "LObjN LO::WIDE_3,           &X, &Y, &W, LN3::MINE_TRACKS_BRAKES"},
  {"T":"MINE_TRACK_UP",                 "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_UP,       &X, &Y"}
  {"T":"MINE_TRACK_UP_LEFT",            "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_UPLEFT,   &X, &Y"}
  {"T":"MINE_TRACK_UP_RIGHT",           "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_UPRIGHT,  &X, &Y"}
  {"T":"MINE_TRACK_BUMP",               "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_BUMP,     &X, &Y"}
  {"T":"MINE_TRACK_SPECIAL",            "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_SPECIAL,  &X, &Y"}
  {"T":"MINE_TRACK_STOP",               "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_STOP,     &X, &Y"}
  {"T":"MINE_TRACK_SUPPORTS",           "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_SUPPORTS, &X, &Y"}
  {"T":"MINE_TRACK_STEEP_LEFT_BOT",     "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_SSL,      &X, &Y"}
  {"T":"MINE_TRACK_STEEP_RIGHT_BOT",    "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_SSR,      &X, &Y"}
  {"T":"MINE_TRACK_GRADUAL_LEFT_L",     "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_SGL,      &X, &Y"}
  {"T":"MINE_TRACK_GRADUAL_RIGHT_R",    "W":1,   "H":1,  "O": "LObj  LO::S_MTRACK_SGR,      &X, &Y"}
  {"T":"MINE_TRACK_SUPPORTS_ONLY",      "W":16,  "H":16, "O": "LObjN LO::R_MTRACK_SUPPORTSONLY,  &X, &Y, &W, &H"},
  {"T":"ENEMY_BARRIER",                 "W":16,  "H":16, "O": "LObjN LO::R_ENEMY_BARRIER      ,  &X, &Y, &W, &H"}
  {"T":"SNOWMAN_TOP",                   "W":1,   "H":1,  "O": "LObj  LO::S_SNOWMAN,         &X, &Y"}
  {"T":"FROZEN_CRACK",                  "W":1,   "H":1,  "O": "LObj  LO::S_FROZENCRACK,     &X, &Y"}
  {"T":"FROZEN_CRACK",                  "W":1,   "H":16, "O": "LObjN LO::TALL_3,            &X, &Y, &H, LN3::FROZEN_CRACK"},
  {"T":"FROZEN_CRACK",                  "W":16,  "H":1,  "O": "LObjN LO::WIDE_3,            &X, &Y, &W, LN3::FROZEN_CRACK"},
  {"T":"ICE2",                          "W":16,  "H":16, "O": "LObjN LO::R_ICE2,            &X, &Y, &W, &H"}
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

// Return an array containing all rectangles in a layer of a given type
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
  local Config = api.GetLevelTbl("Config");
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

  local HScreens = Width/16;
  local VScreens = Height/15;
  local UseLinks = VScreens > 1;

  local ManualScreenLinks = FindType(CT, ["SCREEN_LINK"]);
  local ManualScreenList = array(16, 0);
  local UseManualScreenLinks = ManualScreenLinks.len() > 0;

  foreach(R in ManualScreenLinks) {
    local Args         = split(R[REXTRA], ",");
    local Top          = Args[1];
    local Bottom       = Args[2];

    local ThisScreen   = R[RX]/16;
    local TopScreen    = 0;
    local BottomScreen = 0;

    // Search for links
    foreach(R2 in ManualScreenLinks) {
      local Name = split(R2[REXTRA], ",")[0];
      if(Name == Top) {
        TopScreen = ((R2[RX]/16)-ThisScreen)&15;
      }
      if(Name == Bottom) {
        BottomScreen = ((R2[RX]/16)-ThisScreen)&15;
      }
    }

    ManualScreenList[ThisScreen] = (TopScreen<<4)|BottomScreen;
  }

  // Find all scroll locks
  foreach(R in CT)
    if(R[RTYPE]=="SCROLL_LOCK") {
      local Screen = (R[RX]/16) + (R[RY]/15)*HScreens;
      if((R[RX]&15) == 15)
        SetBoundaryAt(Screen+1);
      else
        SetBoundaryAt(Screen);
    }

  print("Height "+Height)
  print("VScreens "+VScreens)

  // Rearrange level into a big horizontal strip
  // To do: allow the user to skip certain screens
  if(VScreens > 1) {
    foreach(List in [FG, SP, CT])
      for(local i=0; i<List.len(); i++) {
        local R = List[i];
        local V = R[RY] / 15;
        List[i][RX] += V*Width;
        List[i][RY] = List[i][RY] % 15;
      }
  }

  // Find player start position
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

  // Merge FG control types into FG
  FG.extend(FindType(CT, FGControlTypes));

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
  if(UseLinks || UseManualScreenLinks)
    Boundaries = Boundaries | 1; // signal that there are links too
  api.ExportWrite(File, format("  .byt $%.2x, $%.2x ; boundaries", Boundaries>>8, Boundaries&255));

  // write links if used
  if(UseManualScreenLinks) {
    // to do, allow runs of the same link
    for(local i=0; i<16; i++) {
      api.ExportWrite(File, format("  .byt $%.2x, $%.2x ; link", 0, ManualScreenList[i]));
    }
  }
  else if(UseLinks) {
    for(local i=0; i<VScreens; i++) {
      local Up = ((-HScreens)&15)<<4;
      local Down = HScreens;
      if(i==0) Up = 0;
      if(i==VScreens-1) Down = 0;
      api.ExportWrite(File, format("  .byt $%.2x, $%.2x ; link", (HScreens-1)&15, Up|Down));
    }
    if(HScreens*VScreens != 16) // Fill out any unused screens
      api.ExportWrite(File, format("  .byt $%.2x, $00", (16-(HScreens*VScreens)-1)&15));  
  }

  // do the data section
  api.ExportWrite(File, "");
  api.ExportWrite(File, Filename+"Data:");

  // write any config stuff if present
  if(Config) {
    if("StartDialog" in Config)
      api.ExportWrite(File, "  .byt LSpecialCmd, LevelSpecialConfig::SET_START_DIALOG, <"+Config.StartDialog+", >"+Config.StartDialog);
    if("PuzzleMode" in Config)
      api.ExportWrite(File, "  .byt LSpecialCmd, LevelSpecialConfig::PUZZLE_MODE, "+Config.PuzzleMode+", $00");
  }

  // write all level FG commands
  local LastX = 0;
  foreach(R in FG) {
    local Rule = FindRuleFor(R[RTYPE], R[RW], R[RH]);
    if(Rule == -1)
      api.MessageBox(format("No rule found for %s of size %i, %i", R[RTYPE], R[RW], R[RH]));

    // Adjust the X position
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

    // Control layer types that got merged into the FG layer get written too
    if(FGControlTypes.find(R[RTYPE])!=null) {
      api.ExportWrite(File, "  LSetX "+ R[RX]);
      XDifference = 0;
      switch(R[RTYPE]) {
        case "BLOCK_CONTENTS":
        case "COLUMN_DATA":
          api.ExportWrite(File, "  LWriteCol "+R[REXTRA]);
          break;
        case "COLUMN_POINTER":
          api.ExportWrite(File, "  LWriteCol <"+R[REXTRA]+", >"+R[REXTRA]);
          break;
      }
    } else {
      api.ExportWrite(File, "  "+api.Interpolate(RectRules[Rule].O, "", ["X"+XDifference, "Y"+R[RY], "W"+(R[RW]-1), "H"+(R[RH]-1), "T"+R[RTYPE]]));
    }

    // Add extra data
    if(R[REXTRA] && R[REXTRA].len()) {
      local Dashes = split(R[REXTRA], "-");
      local Commas = split(R[REXTRA], ",");
      if(R[RTYPE] == "MINE_TRACK_BRAKES")
        api.ExportWrite(File, "  LWriteCol "+R[REXTRA]);
      if(R[RTYPE] == "CLONER")
        api.ExportWrite(File, "  LWriteCol Enemy::"+R[REXTRA]);
      if(R[RTYPE] == "CLONE_SWITCH") {
        foreach(R2 in CT) {
          if(R2[RTYPE]=="TELEPORT_DESTINATION") {
             if(R2[REXTRA] == R[REXTRA]) {
               api.ExportWrite(File, "  LWriteCol "+R2[RX]);
             }
           }
        }
      }
      if(PrizeContainingTypes.find(R[RTYPE]) != null)
        api.ExportWrite(File, "  LWriteCol InventoryItem::"+R[REXTRA]);
      if(ColDataPointerTypes.find(R[RTYPE]) != null)
        api.ExportWrite(File, "  LWriteCol <"+R[REXTRA]+", >"+R[REXTRA]);
      switch(R[RTYPE]) {
        case "TELEPORT":
        case "DOOR":
        case "TELEPORTER":
          if(Dashes.len() == 2) { // A-B, two paired doors
            local Found = false;
            foreach(R2 in FG) {
              local IsTeleporter = R2[RTYPE] == "TELEPORTER";
              if(R2[RTYPE]=="DOOR" || R2[RTYPE]=="TELEPORTER") {
                local Dashes2 = split(R2[REXTRA], "-");
                if(Dashes2.len() == 2 && Dashes2[0]==Dashes[1]) {
                  api.ExportWrite(File, "  LWriteCol "+(R2[RY]-(IsTeleporter?1:0))+", "+R2[RX]);
                  Found = true;
                }
              }
            }
            if(!Found)
              foreach(R2 in CT) {
                if(R2[RTYPE]=="TELEPORT_DESTINATION") {
                  if(R2[REXTRA] == Dashes[1]) {
                    api.ExportWrite(File, "  LWriteCol "+R2[RY]+", "+R2[RX]);
                    Found = true;
                  }
                }
              }
            if(!Found)
              api.MessageBox("Destination door "+Dashes[1]+" not found");
         } else if(Commas.len() == 2) { // X,Y
           api.ExportWrite(File, "  LWriteCol "+Commas[0]+", "+Commas[1]);
         } else if(Commas.len() == 3) { // X,Y,level
           api.ExportWrite(File, "  LWriteCol $10|"+Commas[0]+", "+Commas[1]+", LevelId::"+Commas[2]);
         } else if(R[REXTRA][0] == '*'){ // script
           api.ExportWrite(File, "  LWriteCol $20, <"+R[REXTRA].slice(1)+", >"+R[REXTRA].slice(1));
         } else if(Commas.len() == 1 && Dashes.len() == 1) {
           api.ExportWrite(File, "  LWriteCol $21, LevelId::"+R[REXTRA])
         } else {
           api.MessageBox("Bad door data: "+R[REXTRA]);
         }
         break;
      }

    }
  }

  // Write background changes
  local BGChanges = FindType(CT, ["BACKGROUND"]);
  for(local i=0; i < BGChanges.len(); i++) {
      local NextX = HScreens*VScreens;
      local ThisX = BGChanges[i][RX]/16;
      if(i != BGChanges.len()-1)
        NextX = BGChanges[i+1][RX]/16;
      if(BGChanges[i][REXTRA] && BGChanges[i][REXTRA].len())
        api.ExportWrite(File, format("  .byt LSpecialCmd, LevelSpecialConfig::MAKE_BACKGROUNDS, $%.2x, LevelBackgroundId::%s",
          (ThisX<<4)|(NextX-ThisX-1), BGChanges[i][REXTRA]));
  }

  local LevelEffects = FindType(CT, ["LEVEL_EFFECT"]);
  foreach(R in LevelEffects) {
    api.ExportWrite(File, format("  .byt LSpecialCmd, LevelSpecialConfig::%s", R[REXTRA]));
  }

  api.ExportWrite(File, "  LFinished");

  // Write the list of sprites
  api.ExportWrite(File, "");
  api.ExportWrite(File, Filename+"Sprite:");
  foreach(sprite in SP) {
    if(sprite[REXTRA] && sprite[REXTRA].len())
      api.ExportWrite(File, format("  LSpr Enemy::%-20s %i, %3i, %3i, %s", sprite[0]+",", (sprite[RFLIPS]&1), sprite[RX], sprite[RY], sprite[REXTRA]));
    else
      api.ExportWrite(File, format("  LSpr Enemy::%-20s %i, %3i, %3i", sprite[0]+",", (sprite[RFLIPS]&1), sprite[RX], sprite[RY]));
  }
  api.ExportWrite(File, "  .byt 255 ; end");

  api.ExportClose(File);
  api.MessageBox("Exported successfully");
  return true;
}

api.RegisterExport(PrincessExport, "PrincessNES");
