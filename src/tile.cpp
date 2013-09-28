#include "tile.h"

Tile::Tile(float x, float y, float w, float h, glhckTexture* texture, glhckRect const& textureTransform, ew::State* state) : ew::Renderable(),
  o(nullptr), x(x), y(y), w(w), h(h), state(state), roles(this, state)
{
  o = glhckSpriteNew(texture, w, h);
  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &textureTransform, 0);
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
}

Tile::~Tile()
{
  glhckObjectFree(o);
}

void Tile::render()
{
  glhckObjectDraw(o);
}
float Tile::getX() const
{
  return x;
}

void Tile::setX(float value)
{
  x = value;
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
}
float Tile::getY() const
{
  return y;
}

void Tile::setY(float value)
{
  y = value;
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
}

glhckObject* Tile::getGlhckObject() const
{
  return o;
}


