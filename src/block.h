#ifndef BLOCK_H
#define BLOCK_H

#include "glhck/glhck.h"

#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/rectblockcollidableblock.h"
#include "ew/state.h"
#include "ew/rolemanager.h"

#include <vector>

class Block : public ew::Updatable, public ew::Renderable, public ew::RectBlockCollidableBlock
{
public:
  struct PathNode
  {
    float x;
    float y;
    float t;
  };

  Block(float x, float y, float w, float h, std::vector<PathNode> const& path, ew::State* state);
  ~Block();

  void render();
  void update(float const delta);

  RectCollisionInformation getRectCollisionInformation();
  void setRectCollisionInformation(const RectCollisionInformation &newRectCollisionInformation);
  void handleRectCollision(RectCollidable *);

private:
  glhckObject* o;
  float x;
  float y;
  float w;
  float h;
  float vx;
  float vy;
  float t;
  bool colliding = false;
  std::vector<PathNode> path;
  unsigned int pathNodeIndex;

  ew::State* state;
  ew::RoleManager<Block, ew::Updatable, ew::Renderable, RectBlockCollidableBlock, RectCollidable> roles;
};

#endif // BLOCK_H
