local RectRules = [
  {"T":"CHECKER",        "W":1,   "H":1,  "O": "db LVL_CHECKER, &P"},
  {"T":"SOLID",          "W":1,   "H":1,  "O": "db LVL_SOLID,   &P"},
  {"T":"DIRT",           "W":1,   "H":1,  "O": "db LVL_DIRT,    &P"},
  {"T":"PLAT",           "W":1,   "H":1,  "O": "db LVL_PLAT,    &P"},
  {"T":"FPLAT",          "W":1,   "H":1,  "O": "db LVL_FPLAT,   &P"},
  {"T":"LADDER",         "W":1,   "H":1,  "O": "db LVL_LADDER,  &P"},
  {"T":"FENCE",          "W":1,   "H":1,  "O": "db LVL_FENCE,   &P"},
  {"T":"GRASS",          "W":1,   "H":1,  "O": "db LVL_GRASS,   &P"},
  {"T":"FLOWER",         "W":1,   "H":1,  "O": "db LVL_FLOWER,  &P"},
  {"T":"LOCK",           "W":1,   "H":1,  "O": "db LVL_LOCK,    &P"},
  {"T":"SPRING",         "W":1,   "H":1,  "O": "db LVL_SPRING,  &P"},

  {"T":"CHECKER",        "W":16,  "H":1,  "O": "db LVL_H_CHECKER|&W, &P"},
  {"T":"CHECKER",        "W":1,   "H":16, "O": "db LVL_V_CHECKER|&H, &P"},

  {"T":"PLAT",          "W":16,  "H":1,  "O": "db LVL_H_PLAT|&W, &P"},
  {"T":"FPLAT",          "W":16,  "H":1,  "O": "db LVL_H_FPLAT|&W, &P"},
  {"T":"FENCE",          "W":16,  "H":1,  "O": "db LVL_H_FENCE|&W, &P"},
  {"T":"GRASS",          "W":16,  "H":1,  "O": "db LVL_H_GRASS|&W, &P"},
  {"T":"FLOWER",          "W":16,  "H":1,  "O": "db LVL_H_FLOWER|&W, &P"},

  {"T":"SOLID",          "W":16,  "H":1,  "O": "db LVL_H_SOLID|&W, &P"},
  {"T":"SOLID",          "W":1,   "H":16, "O": "db LVL_V_SOLID|&H, &P"},
  {"T":"LADDER",         "W":1,   "H":16, "O": "db LVL_V_LADDER|&H, &P"},

  {"T":"EMPTY",          "W":16,  "H":16, "O": "db LVL_RECT|LVL_EMPTY,   &P, &R"},
  {"T":"CHECKER",        "W":16,  "H":16, "O": "db LVL_RECT|LVL_CHECKER, &P, &R"},
  {"T":"SOLID",          "W":16,  "H":16, "O": "db LVL_RECT|LVL_SOLID,   &P, &R"},
  {"T":"DIRT",           "W":16,  "H":16, "O": "db LVL_RECT|LVL_DIRT,    &P, &R"},
  {"T":"PLAT",           "W":16,  "H":16, "O": "db LVL_RECT|LVL_PLAT,    &P, &R"},
  {"T":"FPLAT",          "W":16,  "H":16, "O": "db LVL_RECT|LVL_FPLAT,   &P, &R"},
  {"T":"LADDER",         "W":16,  "H":16, "O": "db LVL_RECT|LVL_LADDER,  &P, &R"},
  {"T":"FENCE",          "W":16,  "H":16, "O": "db LVL_RECT|LVL_FENCE,   &P, &R"},
  {"T":"GRASS",          "W":16,  "H":16, "O": "db LVL_RECT|LVL_GRASS,   &P, &R"},
  {"T":"FLOWER",         "W":16,  "H":16, "O": "db LVL_RECT|LVL_FLOWER,  &P, &R"},
  {"T":"LOCK",           "W":16,  "H":16, "O": "db LVL_RECT|LVL_LOCK,    &P, &R"},
  {"T":"SPRING",         "W":16,  "H":16, "O": "db LVL_RECT|LVL_SPRING,  &P, &R"}
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
  local Header = api.GetLevelTbl("Header");
  local Config = api.GetLevelTbl("Config");

  local Width = api.GetInfo("width");
  local Height = api.GetInfo("height");

  // open the output file
  local Filename = split(api.GetInfo("filename"), ".")[0];
  local ExportName = Filename+".s";
  local File = api.ExportOpen(ExportName);
  if(File < 0) // file couldn't open?
    return false;

  // label for the header
  api.ExportWrite(File, Filename+":");

  // write all level FG commands
  foreach(R in FG) {
    local Rule = FindRuleFor(R[RTYPE], R[RW], R[RH]);
    if(Rule == -1)
      api.MessageBox(format("No rule found for %s of size %i, %i", R[RTYPE], R[RW], R[RH]));

    local PI = "P"+Hex(R[RY]*16+R[RX]);
    local RI = "R"+Hex((R[RH]-1)*16+(R[RW]-1))
    local WI = "W"+Hex(R[RW]-1);
    local HI = "H"+Hex(R[RH]-1);
    api.ExportWrite(File, "  "+api.Interpolate(RectRules[Rule].O, "", [PI, RI, WI, HI]));
  }
  api.ExportWrite(File, "  db LVL_DONE, $00");

  api.ExportClose(File);
  api.MessageBox("Exported successfully");
  return true;
}

api.RegisterExport(PrincessExport, "FHBGGB");
