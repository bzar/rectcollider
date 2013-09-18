#ifndef ENEMYBLOCK_H
#define ENEMYBLOCK_H

#include "block.h"

class EnemyBlock : public Block
{
public:
  EnemyBlock(float x, float y, float w, float h, std::vector<PathNode> const& path, ew::State* state);
};

#endif // ENEMYBLOCK_H
