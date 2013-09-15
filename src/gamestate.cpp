#include "gamestate.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/rectblockcollidephase.h"
#include "ew/rectcollidephase.h"
#include "qmloninitializer.h"
#include "qmloninitializershelpers.h"
#include "ew/engine.h"

GameState::GameState(std::list<std::string> const& levelFilenames) : ew::State(),
  player(nullptr), blocks(), enemies(), levelFilenames(levelFilenames)
{
  phases = {new ew::UpdatePhase(this), new ew::RectBlockCollidePhase(this), new ew::RectCollidePhase(this),
            new GamePhase(this, this), new ew::RenderPhase(this) };

  player = new Player(400, 240, 10, 10, this);
  goal = new Goal(100, 100, 10, 10, this);

  loadLevel(this->levelFilenames.front());
  this->levelFilenames.pop_front();

  /*blocks = {
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

  enemies = {
    new Enemy{120, 20, 10, 10, -40, this},
    new Enemy{470, 90, 10, 10, 49, this}
  };*/
}

GameState::~GameState()
{
  delete player;
  reset();
}

void GameState::enter()
{
  glhckColorb bgColor = {90, 134, 255, 255};
  glhckRenderClearColor(&bgColor);
}

void GameState::gamePhase(const float delta)
{
  if(!player->getAlive())
    player->respawn(400, 240);

  if(player->getVictorious())
  {
    if(levelFilenames.empty())
    {
      engine->quit();
    }
    else
    {
      loadLevel(levelFilenames.front());
      levelFilenames.pop_front();
    }
  }
}

void GameState::loadLevel(const std::string& filename)
{
  qmlon::Initializer<Block::PathNode> pni({
    {"x", qmlon::set(&Block::PathNode::x)},
    {"y", qmlon::set(&Block::PathNode::y)},
    {"t", qmlon::set(&Block::PathNode::t)}
  });

  qmlon::Initializer<GameState> gsi({
    {"start", [&](GameState& gameState, qmlon::Value::Reference v) {
       qmlon::Object& o = v->asObject();
       float x = o.getProperty("x")->asFloat();
       float y = o.getProperty("y")->asFloat();
       gameState.player->respawn(x, y);
    }},
    {"goal", [&](GameState& gameState, qmlon::Value::Reference v) {
       qmlon::Object& o = v->asObject();
       float x = o.getProperty("x")->asFloat();
       float y = o.getProperty("y")->asFloat();
       gameState.goal->moveTo(x, y);
    }}
  }, {
    {"Block", [&](GameState& gameState, qmlon::Object& o) {
       float x = o.getProperty("x")->asFloat();
       float y = o.getProperty("y")->asFloat();
       float w = o.getProperty("w")->asFloat();
       float h = o.getProperty("h")->asFloat();

       std::vector<Block::PathNode> pathNodes;

       if(o.hasProperty("path")) {
         qmlon::Value::List path = o.getProperty("path")->asList();
         for(qmlon::Value::Reference& item : path)
         {
           Block::PathNode pathNode = qmlon::create(item->asObject(), pni);
           pathNodes.push_back(pathNode);
         }
       }

       Block* block = new Block(x, y, w, h, pathNodes, &gameState);
       gameState.blocks.push_back(block);
    }},
    {"Enemy", [&](GameState& gameState, qmlon::Object& o) {
       float x = o.getProperty("x")->asFloat();
       float y = o.getProperty("y")->asFloat();
       float w = o.getProperty("w")->asFloat();
       float h = o.getProperty("h")->asFloat();
       float vx = o.getProperty("vx")->asFloat();

       Enemy* enemy = new Enemy(x, y, w, h, vx, &gameState);
       gameState.enemies.push_back(enemy);
    }},

  });

  qmlon::Value::Reference fileContent = qmlon::readFile(filename);
  reset();
  gsi.init(*this, fileContent);
}

void GameState::reset()
{
  for(Block* block : blocks)
  {
    delete block;
  }
  blocks.clear();

  for(Enemy* enemy : enemies)
  {
    delete enemy;
  }
  enemies.clear();
}
