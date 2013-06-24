#include "rectblockcollidableactor.h"
#include "rectblockcollidableblock.h"

bool RectBlockCollidableActor::horizontalRectBlockCollision(RectBlockCollidableBlock &block, float const delta)
{
  RectCollisionInformation a = getRectCollisionInformation();
  RectCollisionInformation b = block.getRectCollisionInformation();
  RectCollisionInformation result = {
    a.vx > b.vx ? b.x - a.w : b.x + b.w,
    a.y, a.w, a.h, a.vx * b.vx < 0 ? 0 : a.vx, a.vy

  };
  setRectCollisionInformation(result);
  return true;
}

bool RectBlockCollidableActor::verticalRectBlockCollision(RectBlockCollidableBlock &block, const float delta)
{
  RectCollisionInformation a = getRectCollisionInformation();
  RectCollisionInformation b = block.getRectCollisionInformation();
  RectCollisionInformation result = {
    a.x, a.vy > b.vy ? b.y - a.h : b.y + b.h,
    a.w, a.h, a.vx, a.vy * b.vy < 0 ? 0 : a.vy

  };
  setRectCollisionInformation(result);
  return true;
}

void RectBlockCollidableActor::squishRectBlockCollision()
{
}


