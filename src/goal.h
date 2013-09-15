#ifndef GOAL_H
#define GOAL_H

#include "glhck/glhck.h"

#include "ew/updatable.h"
#include "ew/renderable.h"
#include "ew/rectblockcollidableblock.h"
#include "ew/state.h"
#include "ew/rolemanager.h"

class Goal : public ew::Updatable, public ew::Renderable, public ew::RectBlockCollidableBlock
{
public:
  Goal(float x, float y, float w, float h, ew::State* state);
  ~Goal();

  void update(const float delta) override;
  void render() override;

  RectCollisionInformation getRectCollisionInformation() override;
  void setRectCollisionInformation(const RectCollisionInformation &newRectCollisionInformation) override;

  void moveTo(float sx, float sy);

private:
  glhckObject* o;
  float x;
  float y;
  float w;
  float h;

  ew::State* state;
  ew::RoleManager<Goal, ew::Updatable, ew::Renderable, RectBlockCollidableBlock, RectCollidable> roles;
};

#endif // GOAL_H
