#include "enemyblock.h"

EnemyBlock::EnemyBlock(float x, float y, float w, float h, std::vector<PathNode> const& path, ew::State* state)
  : Block(x, y, w, h, path, state)
{
  glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 64, 64, 255);
}
