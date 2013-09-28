#include "block.h"
#include "player.h"
#include <iostream>

Block::Block(float x, float y, float w, float h, ew::State *state) :
  ew::Updatable(), RectBlockCollidableBlock(), o(glhckObjectNew()),
  x(x), y(y), w(w), h(h), vx(0), vy(0),
  t(0), path(), roles(this, state)
{
  glhckObjectPositionf(o, x, y, 0);
}

Block::~Block()
{
}

void Block::update(const float delta)
{
  followPath(delta);

  x += vx * delta;
  y += vy * delta;
  glhckObjectPositionf(o, roundf(x), roundf(y), 0);
  colliding = false;
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

void Block::handleRectCollision(RectCollidable * other)
{
  if(typeid(*other) == typeid(Player))
  {
    colliding = true;
  }
}

void Block::followPath(const float delta)
{
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
}
bool Block::getBlocking() const
{
  return blocking;
}

void Block::setBlocking(bool value)
{
  blocking = value;
}

void Block::addChildTile(Tile* tile)
{
  glhckObjectAddChild(o, tile->getGlhckObject());
  tile->setX(tile->getX() - x);
  tile->setY(tile->getY() - y);
}

bool Block::getLethal() const
{
  return lethal;
}

void Block::setLethal(bool value)
{
  lethal = value;
}


void Block::setPath(const std::vector<PathNode>& value)
{
  path = value;
}


