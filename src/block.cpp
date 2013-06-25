#include "block.h"

Block::Block(float x, float y, float w, float h, std::vector<PathNode> const& path, ew::State *state) :
  ew::Updatable(), ew::Renderable(), RectBlockCollidableBlock(),
  o(nullptr), x(x), y(y), w(w), h(h), vx(0), vy(0), t(0), path(path),
  state(state), roles(this, state)
{
  o = glhckPlaneNew(w, h);
  glhckMaterial* mat = glhckMaterialNew(nullptr);
  glhckMaterialDiffuseb(mat, 128, 128, 128, 255);
  glhckObjectMaterial(o, mat);
  glhckMaterialFree(mat);
  glhckObjectPositionf(o, x, y, 0);
}

Block::~Block()
{
  glhckObjectFree(o);
}

void Block::update(const float delta)
{

  if(colliding)
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 192, 192, 192, 255);
  else
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 128, 128, 128, 255);

  if(path.size() > 1)
  {
    t += delta;
    float pt = t;
    unsigned int i = 1;

    while(pt > 0)
    {
      PathNode const& prev = path.at(i - 1);
      PathNode const& next = path.at(i);
      if(pt <= next.t)
      {
        float const progress = pt / next.t;
        vx = (prev.x + (next.x - prev.x) * progress  - x) / delta;
        vy = (prev.y + (next.y - prev.y) * progress  - y) / delta;
      }
      else
      {
        i = i % (path.size() - 1) + 1;
      }

      pt -= next.t;
    }
  }

  x += vx * delta;
  y += vy * delta;
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
  colliding = false;
}

void Block::render()
{
  glhckObjectDraw(o);
}

ew::RectCollidable::RectCollisionInformation Block::getRectCollisionInformation()
{
  return {x, y, w, h, vx, vy};
}

void Block::setRectCollisionInformation(const RectCollidable::RectCollisionInformation &newRectCollisionInformation)
{
  x = newRectCollisionInformation.x;
  y = newRectCollisionInformation.y;
  vx = newRectCollisionInformation.vx;
  vy = newRectCollisionInformation.vy;
}

void Block::handleRectCollision(RectCollidable *)
{
  colliding = true;
}

