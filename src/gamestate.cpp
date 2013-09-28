#include "gamestate.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/rectblockcollidephase.h"
#include "ew/rectcollidephase.h"

#include "Tmx.h"
#include "tileset.h"

#include "ew/engine.h"
#include <algorithm>

#include <iostream>
#include "glhck/glhck.h"

GameState::GameState(std::vector<std::string> const& levelFilenames) : ew::State(),
  player(nullptr), blocks(), enemies(), levelFilenames(levelFilenames), levelIterator()
{
  phases = {new ew::UpdatePhase(this), new ew::RectBlockCollidePhase(this), new ew::RectCollidePhase(this),
            new GamePhase(this, this), new ew::RenderPhase(this) };

  player = new Player(400, 240, 16, 16, this);
  goal = new Goal(100, 100, 16, 16, this);

  levelIterator = levelFilenames.begin();


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
  loadLevel(*levelIterator);
  glhckColorb bgColor = {90, 134, 255, 255};
  glhckRenderClearColor(&bgColor);
}

void GameState::gamePhase(const float delta)
{
  if(!player->getAlive())
    player->respawn(startX, startY);

  if(player->getVictorious())
  {
    ++levelIterator;
    if(levelIterator == levelFilenames.end())
    {
      engine->quit();
    }
    else
    {
      loadLevel(*levelIterator);
    }
  }
}

void GameState::setLevel(const std::string& filename)
{
  levelIterator = std::find(levelFilenames.begin(), levelFilenames.end(), filename);
}

void GameState::loadLevel(const std::string& filename)
{
  Tmx::Map map;
  map.ParseFile(filename);
  std::vector<Tileset> tilesets;

  for(Tmx::Tileset* tileset : map.GetTilesets())
  {
    tilesets.push_back(Tileset(tileset));
  }

  int tileWidth = map.GetTileWidth();
  int tileHeight = map.GetTileHeight();

  for(Tmx::Layer* layer : map.GetLayers())
  {
    int layerWidth = layer->GetWidth();
    int layerHeight = layer->GetHeight();

    for(int ty = 0; ty < layerHeight; ++ty)
    {
      for(int tx = 0; tx < layerWidth; ++tx)
      {
        Tmx::MapTile const& mapTile = layer->GetTile(tx, ty);

        if(mapTile.tilesetId >= 0)
        {
          int x = tileWidth * tx;
          int y = tileHeight * ty;

          Tileset& tileset = tilesets.at(mapTile.tilesetId);
          Block* block = new Block(x, y, tileWidth, tileHeight, tileset.getTexture(), tileset.getRect(mapTile.id), this);
          Tmx::Tile const* tile = map.GetTileset(mapTile.tilesetId)->GetTile(mapTile.id);

          if(tile)
          {
            const Tmx::PropertySet& properties = tile->GetProperties();
            block->setLethal(properties.HasProperty("lethal") && properties.GetNumericProperty("lethal") != 0);
          }
          blocks.push_back(block);
        }
      }
    }
  }

  for(Tmx::ObjectGroup* objectGroup : map.GetObjectGroups())
  {
    for(Tmx::Object* object : objectGroup->GetObjects())
    {
      int x = object->GetX();
      int y = object->GetY() - map.GetTileHeight();
      if(object->GetType() == "start")
      {
        startX = x;
        startY = y;
        player->respawn(startX, startY);
      }
      else if(object->GetType() == "goal")
      {
        goal->moveTo(x, y);
      }
      else if(object->GetType() == "block")
      {
        int tilesetIndex = map.FindTilesetIndex(object->GetGid());
        int tileId = object->GetGid() - map.GetTileset(tilesetIndex)->GetFirstGid();
        Tileset& tileset = tilesets.at(tilesetIndex);
        Block* block = new Block(x, y, tileWidth, tileHeight, tileset.getTexture(), tileset.getRect(tileId), this);
        Tmx::Tile const* tile = map.GetTileset(tilesetIndex)->GetTile(tileId);

        if(tile)
        {
          const Tmx::PropertySet& properties = tile->GetProperties();
          block->setLethal(properties.HasProperty("lethal") && properties.GetNumericProperty("lethal") != 0);
        }
        if(object->GetProperties().HasProperty("path"))
        {
          std::cout << "block has path" << std::endl;
          std::string pathName = object->GetProperties().GetLiteralProperty("path");
          auto iter = std::find_if(objectGroup->GetObjects().begin(), objectGroup->GetObjects().end(), [&pathName](Tmx::Object* o) {
            return o->GetName() == pathName;
          });

          if(iter != objectGroup->GetObjects().end())
          {
            std::cout << "path object found" << std::endl;
            Tmx::Object* path = *iter;
            const Tmx::Polygon* polygon = path->GetPolygon();
            const Tmx::Polyline* polyline = path->GetPolyline();

            auto getPoint = [polygon, polyline](int i) { return polygon ? polygon->GetPoint(i) : polyline->GetPoint(i); };
            int numPoints = polygon ? polygon->GetNumPoints() : polyline->GetNumPoints();

            float duration = path->GetProperties().HasProperty("duration") ? path->GetProperties().GetFloatProperty("duration") : 5.0f;
            float length = 0;
            for(int i = 1; i <= numPoints; ++i)
            {
              const Tmx::Point& a = getPoint((i - 1) % numPoints);
              const Tmx::Point& b = getPoint(i % numPoints);
              length += sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
            }
            std::vector<Block::PathNode> pathNodes;
            for(int i = 0; i <= numPoints; ++i)
            {
              const Tmx::Point& point = getPoint(i % numPoints);
              float px = static_cast<float>(point.x) + path->GetX() + (block->getX() - path->GetX());
              float py = static_cast<float>(point.y) + path->GetY() + (block->getY() - path->GetY());
              float t = 0;
              const Tmx::Point& a = getPoint((i - 1) % numPoints);
              const Tmx::Point& b = getPoint(i % numPoints);
              double l = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
              t = duration * l / length;
              pathNodes.push_back({px, py, t});
            }
            block->setPath(pathNodes);
          }
        }
        blocks.push_back(block);
      }
    }
  }
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
