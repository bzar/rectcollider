#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ew/state.h"
#include "player.h"
#include "block.h"

#include <vector>

class GameState : public ew::State
{
public:
  GameState();
  ~GameState();

private:
  Player* player;
  std::vector<Block*> blocks;
};

#endif // GAMESTATE_H
