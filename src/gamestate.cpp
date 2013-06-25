#include "gamestate.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/rectblockcollidephase.h"
#include "ew/rectcollidephase.h"

GameState::GameState() : ew::State(),
  player(nullptr), blocks()
{
  phases = {new ew::UpdatePhase(this), new ew::RectBlockCollidePhase(this), new ew::RectCollidePhase(this),
            new GamePhase(this, this), new ew::RenderPhase(this) };

  player = new Player(400, 240, 10, 10, this);
  blocks = {
    new Block{0, 0, 10, 480, {}, this},
    new Block{800 - 10, 0, 10, 480, {}, this},
    new Block{11, 0, 800 - 22, 10, {}, this},
    new Block{11, 480 - 10, 800 - 22, 10, {}, this},
    new Block{70, 70, 150, 150, {{70, 70, 0}, {270, 270, 5}, {70, 70, 7}}, this},
    new Block{470, 100, 150, 10, {{470, 100, 0}, {470, 300, 3}, {470, 100, 3}}, this},
    new Block{490, 90, 130, 10, {}, this},
    new Block{470, 310, 180, 10, {}, this},
    new Block{10, 70, 50, 10, {{11, 70, 0}, {11, 70, 1}, {11, 480 - 21, 2}, {11, 480 - 21, 1}, {11, 70, 2}}, this},
  };
}

GameState::~GameState()
{
  delete player;
  for(Block* block : blocks)
  {
    delete block;
  }
}

void GameState::gamePhase(const float delta)
{
  if(!player->getAlive())
    player->respawn(400, 240);
}
