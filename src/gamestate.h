#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "ew/state.h"
#include "ew/customphase.h"

#include "player.h"
#include "block.h"
#include "tile.h"
#include "enemy.h"
#include <vector>
#include <string>

class GameState : public ew::State
{
public:
  GameState(std::string const& levelPath, std::string const& firstLevel);
  ~GameState();

  void enter() override;

  void gamePhase(const float delta);

  void loadLevel(std::string const& filename, int entranceId = 0);

private:
  void reset();

  typedef ew::CustomPhase<GameState, &GameState::gamePhase> GamePhase;

  std::string levelPath;
  std::string firstLevel;

  float startX;
  float startY;
  Player* player;
  std::vector<Tile*> tiles;
  std::vector<Block*> blocks;
  std::vector<Enemy*> enemies;
};

#endif // GAMESTATE_H
