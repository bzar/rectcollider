#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ew/state.h"
#include "ew/customphase.h"

#include "player.h"
#include "goal.h"
#include "block.h"
#include "enemy.h"
#include <vector>
#include <string>
#include <list>

class GameState : public ew::State
{
public:
  GameState(std::list<std::string> const& levelFilenames);
  ~GameState();

  void enter() override;

  void gamePhase(const float delta);

  void loadLevel(std::string const& filename);

private:
  void reset();
  typedef ew::CustomPhase<GameState, &GameState::gamePhase> GamePhase;
  Player* player;
  Goal* goal;
  std::vector<Block*> blocks;
  std::vector<Enemy*> enemies;
  std::list<std::string> levelFilenames;
};

#endif // GAMESTATE_H
