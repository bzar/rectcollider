#include "rectblockcollidephase.h"
#include "rectblockcollidableactor.h"
#include "rectblockcollidableblock.h"

static float const EPSILON = 0.001;
static float const ONE_PLUS_EPSILON = 1 + EPSILON;

RectBlockCollidePhase::RectBlockCollidePhase(ew::State *state) : ew::Phase(state)
{
}

void RectBlockCollidePhase::execute(const float delta)
{
  for(RectBlockCollidableActor* actor : state->entities.get<RectBlockCollidableActor>())
  {
    bool collisions = false;
    int squishCounter = 100;
    do
    {
      collisions = false;

      for(RectBlockCollidableBlock* block : state->entities.get<RectBlockCollidableBlock>())
      {
        RectCollidable::RectCollisionInformation a = actor->getRectCollisionInformation();
        RectCollidable::RectCollisionInformation b = block->getRectCollisionInformation();

        if(rectsCollide(a.x, a.y, a.w, a.h,
                        b.x, b.y, b.w, b.h))
        {
          if(rectsCollide(a.x - a.vx * delta * ONE_PLUS_EPSILON, a.y, a.w, a.h,
                          b.x - b.vx * delta * ONE_PLUS_EPSILON, b.y, b.w, b.h))
          {
            collisions = actor->verticalRectBlockCollision(*block, delta);
          }
          else
          {
            collisions = actor->horizontalRectBlockCollision(*block, delta);
          }

          if(collisions)
          {
            break;
          }
        }
      }
      --squishCounter;
    } while (collisions && squishCounter > 0);

    if(squishCounter <= 0)
    {
      actor->squishRectBlockCollision();
    }
  }
}


bool RectBlockCollidePhase::rectsCollide(float x1, float y1, float w1, float h1,
                                         float x2, float y2, float w2, float h2)
{
  return x1 < x2 + w2 && y1 < y2 + h2 &&
         x2 < x1 + w1 && y2 < y1 + h1;
}
