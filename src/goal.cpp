#include "goal.h"

#include "ew/engine.h"

Goal::Goal(float x, float y, float w, float h, ew::State* state) :
  ew::Updatable(), ew::Renderable(), RectCollidable(),
  o(nullptr), x(x), y(y), w(w), h(h),
  state(state), roles(this, state)
{
  o = glhckPlaneNew(w, h);
  glhckMaterial* mat = glhckMaterialNew(nullptr);
  glhckMaterialDiffuseb(mat, 128, 255, 128, 255);
  glhckObjectMaterial(o, mat);
  glhckMaterialFree(mat);
  glhckObjectPositionf(o, x, y, 0);
}

Goal::~Goal()
{
  glhckObjectFree(o);
}

void Goal::update(const float delta)
{
}

void Goal::render()
{
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
  glhckObjectDraw(o);
}

ew::RectCollidable::RectCollisionInformation Goal::getRectCollisionInformation()
{
  return {x, y, w, h, 0, 0};
}

void Goal::setRectCollisionInformation(const RectCollidable::RectCollisionInformation &newRectCollisionInformation)
{
  x = newRectCollisionInformation.x;
  y = newRectCollisionInformation.y;
}

void Goal::moveTo(float sx, float sy)
{
  x = sx;
  y = sy;
}
