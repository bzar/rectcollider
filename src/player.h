#ifndef PLAYER_H
#define PLAYER_H

#include "glhck/glhck.h"

#include "ew/updatable.h"
#include "ew/renderable.h"
#include "ew/rectblockcollidableactor.h"
#include "ew/state.h"
#include "ew/rolemanager.h"

class Player : public ew::Updatable, public ew::Renderable, public ew::RectBlockCollidableActor
{
public:
  Player(float x, float y, float w, float h, ew::State* state);
  ~Player();

  void update(const float delta) override;
  void render() override;

  RectCollisionInformation getRectCollisionInformation() override;
  void setRectCollisionInformation(const RectCollisionInformation &newRectCollisionInformation) override;
  void handleRectCollision(RectCollidable *other) override;

  void squishRectBlockCollision() override;
  bool verticalRectBlockCollision(ew::RectBlockCollidableBlock &block, float const delta) override;

  bool getAlive() const;
  void respawn(float sx, float sy);

  bool getVictorious() const;
  void setVictorious(bool value);

private:
  glhckObject* o;
  float x;
  float y;
  float w;
  float h;
  float vx;
  float vy;
  bool colliding = false;
  bool onGround = false;
  bool alive = true;
  bool victorious = false;

  ew::State* state;
  ew::RoleManager<Player, ew::Updatable, ew::Renderable, RectBlockCollidableActor, RectCollidable> roles;
};

#endif // PLAYER_H
