#include "Ship.hpp"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

Ship::Ship(ULL hp, vector<ULL> pos) {
  cout << "A new ship has been created" << endl;
}

Ship::~Ship() {
  cout << "A ship has been destroyed" << endl;
}

ULL Ship::GetHP() {
  return HP;
}

void Ship::SetHP(ULL hp) {
  HP = hp;
}

vector<ULL> Ship::GetPos() {
  return Pos;
}

void Ship::SetPos(vector<ULL> pos) {
  Pos = pos;
}
