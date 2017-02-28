#include "SolarSemiotics.hpp"
#include "Ship.hpp"
#include "Tile.hpp"
#include "Window.hpp"
#include <iostream>
#include <SDL.h>

using namespace std;

SolarSemioticsApp::SolarSemioticsApp(std::string title, std::string version)
: Title(title), Version(version) {
  cout << "Starting " << Title << " " << Version << "..." << endl;
  cout << "Starting GUI subsystems" << endl;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    cout << "SDL_Init Error: " << SDL_GetError() << endl;
    return;
  }
  cout << "Opening SDL2 Window" << endl;
  Win = new Window();
  cout << "Loading map..." << endl;
  bool mapLoaded = LoadMap();
  if (!mapLoaded) {
    cerr << "Failed loading map" << endl;
    return;
  }
  cout << "Adding ships..." << endl;
  Ships.push_back(new Ship());
  // cout << "Drawing picture" << endl;
  cout << "Starting event handling subsystems" << endl;
  HandleEvents();
}

SolarSemioticsApp::~SolarSemioticsApp() {
  cout << "Stopping " << Title << " " << Version << endl;
  cout << "Closing SDL2 window" << endl;
  delete Win;
  cout << "Stopping GUI subsystems" << endl;
  SDL_Quit();
  cout << "Closing map" << endl;
  CloseMap();
  cout << "Deleting ships..." << endl;
  for (auto const& ship: Ships) {
    delete ship;
  }
}

bool SolarSemioticsApp::LoadMap(ULL w, ULL h) {
  CloseMap();
  for (ULL h1 = 0; h1 < h; h1++) {
    vector<Tile*> v;
    for (ULL w1 = 0; w1 < w; w1++) {
      v.push_back(new Tile());
    }
    Map.push_back(v);
  }
  return true;
}

void SolarSemioticsApp::CloseMap() {
  for (auto const& h: Map) {
    for (auto const& t: h) {
      delete t;
    }
  }
}

bool SolarSemioticsApp::HandleEvents() {
  bool quit = false;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // Win->Render();
      switch (event.type) {
        case SDL_QUIT:
          quit = true;
          break;
      }
    }
  }
}
