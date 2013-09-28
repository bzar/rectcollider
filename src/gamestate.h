#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ew/state.h"
#include "ew/customphase.h"

#include "player.h"
#include "goal.h"
#include "block.h"
#include "tile.h"
#include "enemy.h"
#include <vector>
#include <string>

class GameState : public ew::State
{
public:
  GameState(std::vector<std::string> const& levelFilenames);
  ~GameState();

  void enter() override;

  void gamePhase(const float delta);

  void setLevel(std::string const& filename);
  void loadLevel(std::string const& filename);

private:
  void reset();

  typedef ew::CustomPhase<GameState, &GameState::gamePhase> GamePhase;
  float startX;
  float startY;
  Player* player;
  Goal* goal;
  std::vector<Tile*> tiles;
  std::vector<Block*> blocks;
  std::vector<Enemy*> enemies;
  std::vector<std::string> levelFilenames;
  std::vector<std::string>::const_iterator levelIterator;
};

#endif // GAMESTATE_H
