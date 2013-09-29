#include "exit.h"

Exit::Exit(std::string const& destination, int const id, float x, float y, float w, float h, ew::State* state) : ew::RectCollidable(),
  destination(destination), id(id), x(x), y(y), w(w), h(h), state(state), roles(this, state)
{
}

ew::RectCollidable::RectCollisionInformation Exit::getRectCollisionInformation()
{
  return {x, y, w, h, 0, 0};
}

void Exit::setRectCollisionInformation(const ew::RectCollidable::RectCollisionInformation& newRectCollisionInformation)
{

}
std::string Exit::getDestination() const
{
  return destination;
}

void Exit::setDestination(const std::string& value)
{
  destination = value;
}
int Exit::getId() const
{
  return id;
}

void Exit::setId(int value)
{
  id = value;
}


