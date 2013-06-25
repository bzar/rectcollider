#ifndef ENEMY_H
#define ENEMY_H

#include "glhck/glhck.h"

#include "ew/updatable.h"
#include "ew/renderable.h"
#include "ew/rectblockcollidableactor.h"
#include "ew/state.h"
#include "ew/rolemanager.h"

class Enemy : public ew::Updatable, public ew::Renderable, public ew::RectBlockCollidableActor
{
public:
  Enemy(float x, float y, float w, float h, float vx, ew::State* state);
  ~Enemy();

  void update(const float delta) override;
  void render() override;

  RectCollisionInformation getRectCollisionInformation() override;
  void setRectCollisionInformation(const RectCollisionInformation &newRectCollisionInformation) override;
  bool verticalRectBlockCollision(ew::RectBlockCollidableBlock &block, float const delta) override;
  bool horizontalRectBlockCollision(ew::RectBlockCollidableBlock &block, float const delta) override;

  void squishRectBlockCollision() override;

private:
  void turnIfNoFloorAhead();

  glhckObject* o;
  float x;
  float y;
  float w;
  float h;
  float vx;
  float vy;

  float xInit;
  float yInit;

  ew::State* state;
  ew::RoleManager<Enemy, ew::Updatable, ew::Renderable, RectBlockCollidableActor, RectCollidable> roles;
};

#endif // ENEMY_H
