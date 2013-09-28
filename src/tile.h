#ifndef TILE_H
#define TILE_H

#include "glhck/glhck.h"
#include "ew/renderable.h"
#include "ew/state.h"

class Tile : public ew::Renderable
{
public:
  Tile(float x, float y, float w, float h, glhckTexture* texture, glhckRect const& textureTransform, ew::State* state);
  ~Tile();
  void render() override;

  float getX() const;
  void setX(float value);

  float getY() const;
  void setY(float value);

protected:
  glhckObject* o;
  float x;
  float y;
  float w;
  float h;
  ew::State* state;

};

#endif // TILE_H
