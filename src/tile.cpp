#include "tile.h"

Tile::Tile(float x, float y, float w, float h, glhckTexture* texture, glhckRect const& textureTransform, ew::State* state) : ew::Renderable(),
  o(nullptr), x(x), y(y), w(w), h(h), state(state)
{
  o = glhckSpriteNew(texture, w, h);
  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &textureTransform, 0);
  glhckObjectPositionf(o, x, y, 0);
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
}
float Tile::getY() const
{
  return y;
}

void Tile::setY(float value)
{
  y = value;
}


