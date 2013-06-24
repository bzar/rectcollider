#ifndef RECTBLOCKCOLLIDABLEACTOR_H
#define RECTBLOCKCOLLIDABLEACTOR_H

#include "rectcollidable.h"

class RectBlockCollidableBlock;

class RectBlockCollidableActor : public RectCollidable
{
public:
  virtual bool horizontalRectBlockCollision(RectBlockCollidableBlock& block, float const delta);
  virtual bool verticalRectBlockCollision(RectBlockCollidableBlock& block, float const delta);
  virtual void squishRectBlockCollision();
};

#endif // RECTBLOCKCOLLIDABLEACTOR_H
