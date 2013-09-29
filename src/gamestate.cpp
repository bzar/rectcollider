#include "gamestate.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/rectblockcollidephase.h"
#include "ew/rectcollidephase.h"

#include "Tmx.h"

#include "tileset.h"
#include "tile.h"
#include "exit.h"

#include "ew/engine.h"
#include <algorithm>

#include <iostream>
#include "glhck/glhck.h"
#include <boost/filesystem.hpp>

namespace
{
  class PropertyUnion
  {
  public:
    PropertyUnion(std::vector<Tmx::PropertySet> sets) : sets(sets) {}
    std::string getString(std::string const& name, std::string const& defaultValue = "")
    {
      for(Tmx::PropertySet const& set : sets)
      {
        if(set.HasProperty(name))
        {
          return set.GetLiteralProperty(name);
        }
      }
      return defaultValue;
    }

    int getInteger(std::string const& name, int defaultValue = 0)
    {
      for(Tmx::PropertySet const& set : sets)
      {
        if(set.HasProperty(name))
        {
          return set.GetNumericProperty(name);
        }
      }
      return defaultValue;
    }

    int getFloat(std::string const& name, float defaultValue = 0.0f)
    {
      for(Tmx::PropertySet const& set : sets)
      {
        if(set.HasProperty(name))
        {
          return set.GetFloatProperty(name);
        }
      }
      return defaultValue;
    }

  private:
    std::vector<Tmx::PropertySet> sets;
  };
}

GameState::GameState(std::string const& levelPath, std::string const& firstLevel) : ew::State(),
  levelPath(levelPath), firstLevel(firstLevel), player(nullptr), exits(), tiles(), blocks(), enemies()
{
  phases = {new ew::UpdatePhase(this), new ew::RectCollidePhase(this), new ew::RectBlockCollidePhase(this),
            new GamePhase(this, this), new ew::RenderPhase(this) };

  player = new Player(400, 240, 16, 16, this);
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
    player->respawn(startX, startY);

  for(auto f : onGamePhaseFuncs)
  {
    f();
  }
  onGamePhaseFuncs.clear();
}

void GameState::loadLevel(const std::string& filename, int entranceId)
{
  onGamePhaseFuncs.push_back([&, filename, entranceId]() {
    if(filename.empty())
    {
      engine->quit();
      return;
    }
    reset();
    boost::filesystem::path path = boost::filesystem::path(levelPath) / boost::filesystem::path(filename);

    Tmx::Map map;
    map.ParseFile(path.generic_string());
    std::vector<Tileset> tilesets;

    for(Tmx::Tileset* tileset : map.GetTilesets())
    {
      tilesets.push_back(Tileset(tileset, levelPath));
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
            Tile* tile = new Tile(x, y, tileWidth, tileHeight, tileset.getTexture(), tileset.getRect(mapTile.id), this);
            tiles.push_back(tile);
          }
        }
      }
    }

    std::vector<std::tuple<Tile*, std::string>> tileParents;
    std::unordered_map<std::string, Block*> namedBlocks;

    for(Tmx::ObjectGroup* objectGroup : map.GetObjectGroups())
    {
      std::string const layerType = objectGroup->GetProperties().HasProperty("type") ? objectGroup->GetProperties().GetLiteralProperty("type") : "";
      bool layerLethal = objectGroup->GetProperties().HasProperty("block") && objectGroup->GetProperties().GetNumericProperty("lethal") == 1;

      for(Tmx::Object* object : objectGroup->GetObjects())
      {
        int x = object->GetX();
        int y = object->GetY();
        std::string const& type = object->GetType().empty() ? layerType : object->GetType();
        PropertyUnion props({object->GetProperties(), objectGroup->GetProperties()});

        if(type == "Entrance")
        {
          int id = props.getInteger("id", 0);
          if(id == entranceId)
          {
            startX = x;
            startY = y;
            player->respawn(startX, startY);
          }
        }
        else if(type == "Exit")
        {
          int id = props.getInteger("id", 0);
          std::string destination = props.getString("destination", "");
          exits.push_back(new Exit(destination, id, x, y, object->GetWidth(), object->GetHeight(), this));
        }
        else if(type == "Block" )
        {
          Block* block = new Block(x, y, object->GetWidth(), object->GetHeight(), this);
          if(!object->GetName().empty())
          {
            namedBlocks[object->GetName()] = block;
          }

          bool lethal = props.getInteger("lethal", 0) == 1;
          block->setLethal(lethal);

          if(object->GetProperties().HasProperty("path"))
          {
            std::string pathName = object->GetProperties().GetLiteralProperty("path");
            auto iter = std::find_if(objectGroup->GetObjects().begin(), objectGroup->GetObjects().end(), [&pathName](Tmx::Object* o) {
              return o->GetName() == pathName;
            });

            if(iter != objectGroup->GetObjects().end())
            {
              Tmx::Object* path = *iter;
              const Tmx::Polygon* polygon = path->GetPolygon();
              const Tmx::Polyline* polyline = path->GetPolyline();

              auto getPoint = [polygon, polyline](int i) { return polygon ? polygon->GetPoint(i) : polyline->GetPoint(i); };
              int numPoints = polygon ? polygon->GetNumPoints() : polyline->GetNumPoints();

              float duration = props.getFloat("duration", 5.0f);
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
                float px = static_cast<float>(point.x) + path->GetX() + (x - path->GetX());
                float py = static_cast<float>(point.y) + path->GetY() + (y - path->GetY());
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
        else if(object->GetGid() != 0)
        {
          int tilesetIndex = map.FindTilesetIndex(object->GetGid());
          int tileId = object->GetGid() - map.GetTileset(tilesetIndex)->GetFirstGid();
          Tileset& tileset = tilesets.at(tilesetIndex);
          Tile* tile = new Tile(x, y - map.GetTileHeight(), tileWidth, tileHeight, tileset.getTexture(), tileset.getRect(tileId), this);
          if(object->GetProperties().HasProperty("parent"))
          {
            tileParents.push_back(make_tuple(tile, object->GetProperties().GetLiteralProperty("parent")));
          }
          tiles.push_back(tile);
        }
      }
    }

    for(auto tp : tileParents)
    {
      Tile* tile = std::get<0>(tp);
      auto blockIter = namedBlocks.find(std::get<1>(tp));
      if(blockIter != namedBlocks.end())
      {
        Block* block = blockIter->second;
        block->addChildTile(tile);
      }
    }
  });
}

void GameState::reset()
{
  for(Exit* exit : exits)
  {
    delete exit;
  }
  exits.clear();

  for(Tile* tile : tiles)
  {
    delete tile;
  }
  tiles.clear();

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
