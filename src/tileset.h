#ifndef TILESET_H
#define TILESET_H

#include "glhck/glhck.h"
#include "TmxTileset.h"
#include <string>
#include <vector>

class Tileset
{
public:
  Tileset(Tmx::Tileset* tileset, std::string const& path);
  Tileset(Tileset const& other);
  ~Tileset();
  glhckRect getRect(int const tileId) const;
  glhckTexture* getTexture() const;
private:
  glhckTexture* texture;
  int imageWidth;
  int imageHeight;
  int tileWidth;
  int tileHeight;
  int firstId;
  int margin;
  int spacing;

};

#endif // TILESET_H
