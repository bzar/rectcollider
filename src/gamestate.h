#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ew/state.h"
#include "ew/customphasehandler.h"

#include "player.h"
#include "block.h"
#include <vector>

class GameState : public ew::State, public ew::CustomPhaseHandler
{
public:
  GameState();
  ~GameState();

  void executeCustomPhase(const float delta, State *state);

private:
  Player* player;
  std::vector<Block*> blocks;
};

#endif // GAMESTATE_H
