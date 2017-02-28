#ifndef __SOLARSEMIOTICS__
#define __SOLARSEMIOTICS__

#include <string>
#include <vector>

class Ship;
class Tile;
class Window;

typedef unsigned long long ULL;

class SolarSemioticsApp {

public:
  std::string Title;
  std::string Version;
  std::vector<Ship*> Ships;
  std::vector<std::vector<Tile*>> Map;
  Window* Win;

  SolarSemioticsApp(std::string title = "Solar Semiotics", std::string version = "1.0.0");
  ~SolarSemioticsApp();
  bool LoadMap(ULL w = 3, ULL h = 3);
  void CloseMap();
  bool HandleEvents();
};

#endif
