#ifndef SOLARSEMIOTICS_SHIP
#define SOLARSEMIOTICS_SHIP

#include <vector>

typedef unsigned long long ULL;

using namespace std;

class Ship {

  ULL HP;
  vector<ULL> Pos;

public:
  Ship(ULL hp = 100, vector<ULL> pos = {0,0});
  ~Ship();
  ULL GetHP();
  void SetHP(ULL hp);
  vector<ULL> GetPos();
  void SetPos(vector<ULL> pos);
};

#endif
