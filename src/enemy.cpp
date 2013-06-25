#include "enemy.h"

#include "ew/engine.h"
#include "ew/rectblockcollidableblock.h"

#include <iostream>

Enemy::Enemy(float x, float y, float w, float h, ew::State* state) :
  ew::Updatable(), ew::Renderable(), RectBlockCollidableActor(),
  o(nullptr), x(x), y(y), w(w), h(h), vx(-40), vy(0),
  state(state), roles(this, state)
{
  o = glhckPlaneNew(w, h);
  glhckMaterial* mat = glhckMaterialNew(nullptr);
  glhckMaterialDiffuseb(mat, 255, 128, 128, 255);
  glhckObjectMaterial(o, mat);
  glhckMaterialFree(mat);
  glhckObjectPositionf(o, x, y, 0);
}

Enemy::~Enemy()
{
  glhckObjectFree(o);
}

void Enemy::update(const float delta)
{
  vy = min(vy + delta * 3000, 500);

  turnIfNoFloorAhead();
  x += vx * delta;
  y += vy * delta;
}

void Enemy::render()
{
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
  glhckObjectDraw(o);
}

ew::RectCollidable::RectCollisionInformation Enemy::getRectCollisionInformation()
{
  return {x, y, w, h, vx, vy};
}

void Enemy::setRectCollisionInformation(const RectCollidable::RectCollisionInformation &newRectCollisionInformation)
{
  x = newRectCollisionInformation.x;
  y = newRectCollisionInformation.y;
  vx = newRectCollisionInformation.vx;
  vy = newRectCollisionInformation.vy;
}

void Enemy::squishRectBlockCollision()
{
}

void Enemy::turnIfNoFloorAhead()
{
  bool floorAhead = false;
  RectCollisionInformation a = {
    vx > 0 ? x + w + 1 : x - 1,
    y + h + 1, 1, 1, 0, 0
  };

  for(ew::RectBlockCollidableBlock* block : state->entities.get<ew::RectBlockCollidableBlock>())
  {
    RectCollisionInformation b =  block->getRectCollisionInformation();
    if(a.collidesWith(b))
    {
      floorAhead = true;
      break;
    }
  }

  if(!floorAhead)
  {
    vx = -vx;
  }
}

bool Enemy::verticalRectBlockCollision(ew::RectBlockCollidableBlock &block, const float delta)
{
  RectCollisionInformation b =  block.getRectCollisionInformation();
  x += b.vx * delta;
  return RectBlockCollidableActor::verticalRectBlockCollision(block, delta);
}

bool Enemy::horizontalRectBlockCollision(ew::RectBlockCollidableBlock &block, const float delta)
{
  float speed = vx;
  bool retval = RectBlockCollidableActor::horizontalRectBlockCollision(block, delta);
  vx = -speed;
}
