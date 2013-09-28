#ifndef BLOCK_H
#define BLOCK_H

#include "glhck/glhck.h"

#include "ew/updatable.h"
#include "ew/rectblockcollidableblock.h"
#include "ew/state.h"
#include "ew/rolemanager.h"
#include "tile.h"

#include <vector>

class Block : public ew::Updatable, public ew::RectBlockCollidableBlock
{
public:
  struct PathNode
  {
    float x;
    float y;
    float t;
  };

  Block(float x, float y, float w, float h, ew::State* state);
  ~Block();

  void update(float const delta);

  RectCollisionInformation getRectCollisionInformation();
  void setRectCollisionInformation(const RectCollisionInformation &newRectCollisionInformation);
  void handleRectCollision(RectCollidable *other);

  void setPath(const std::vector<PathNode>& value);

  bool getLethal() const;
  void setLethal(bool value);

  bool getBlocking() const;
  void setBlocking(bool value);

  void addChildTile(Tile* tile);

protected:
  void followPath(float const delta);

  glhckObject* o;

  float x;
  float y;
  float w;
  float h;
  float vx;
  float vy;
  float t;
  bool colliding = false;
  bool lethal = false;
  bool blocking = false;
  std::vector<PathNode> path;
  unsigned int pathNodeIndex;

  ew::RoleManager<Block, ew::Updatable, RectBlockCollidableBlock, RectCollidable> roles;
};

#endif // BLOCK_H
