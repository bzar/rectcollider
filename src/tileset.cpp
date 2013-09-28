#include "tileset.h"
#include "TmxImage.h"
#include <iostream>

Tileset::Tileset(Tmx::Tileset* tileset) :
  texture(nullptr), imageWidth(tileset->GetImage()->GetWidth()), imageHeight(tileset->GetImage()->GetHeight()),
  tileWidth(tileset->GetTileWidth()), tileHeight(tileset->GetTileHeight()),
  firstId(tileset->GetFirstGid()), margin(tileset->GetMargin()), spacing(tileset->GetSpacing())
{
  const Tmx::Image* image = tileset->GetImage();
  const std::string source = image->GetSource();

  texture = glhckTextureNewFromFile(source.data(), glhckImportDefaultImageParameters(), glhckTextureDefaultSpriteParameters());
}

Tileset::Tileset(const Tileset& other) :
  texture(other.texture), imageWidth(other.imageWidth), imageHeight(other.imageHeight),
  tileWidth(other.tileWidth), tileHeight(other.tileHeight), firstId(other.firstId),
  margin(other.margin), spacing(other.spacing)
{
  glhckTextureRef(texture);
}

Tileset::~Tileset()
{
  glhckTextureFree(texture);
}

glhckRect Tileset::getRect(const int tileId) const
{
  int tilesPerRow = (imageWidth - margin) / (tileWidth + spacing);
  int row = tileId / tilesPerRow;
  int col = tileId - tilesPerRow * row;
  float x = (margin + col * (tileWidth + spacing)) / static_cast<float>(imageWidth);
  float y = (margin + row * (tileHeight + spacing)) / static_cast<float>(imageHeight);
  float w = tileWidth / static_cast<float>(imageWidth);
  float h = tileHeight / static_cast<float>(imageHeight);
  return {x, 1-y, w, h};
}

glhckTexture*Tileset::getTexture() const
{
  return texture;
}
