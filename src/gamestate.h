#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ew/state.h"
#include "ew/customphase.h"

#include "player.h"
#include "block.h"
#include <vector>

class GameState : public ew::State
{
public:
  GameState();
  ~GameState();

  void gamePhase(const float delta);

private:
  typedef ew::CustomPhase<GameState, &GameState::gamePhase> GamePhase;
  Player* player;
  std::vector<Block*> blocks;
};

#endif // GAMESTATE_H
