#include "player.h"

#include "ew/engine.h"
#include "GLFW/glfw3.h"
#include "glfwcontext.h"
#include "ew/rectblockcollidableblock.h"

#include "block.h"
#include "enemy.h"
#include "goal.h"

Player::Player(float x, float y, float w, float h, ew::State* state) :
  ew::Updatable(), ew::Renderable(), RectBlockCollidableActor(),
  o(nullptr), x(x), y(y), w(w), h(h), vx(0), vy(0),
  state(state), roles(this, state)
{
  o = glhckPlaneNew(w, h);
  glhckMaterial* mat = glhckMaterialNew(nullptr);
  glhckMaterialDiffuseb(mat, 255, 160, 100, 255);
  glhckObjectMaterial(o, mat);
  glhckMaterialFree(mat);
  glhckObjectPositionf(o, x, y, 0);
}

Player::~Player()
{
  glhckObjectFree(o);
}

void Player::update(const float delta)
{
  if(!alive)
    return;

  GLFWContext* ctx = state->engine->singletons.get<GLFWContext>();

  vx = 0;
  if(onGround)
    vy = 0;
  else
    vy += 2000 * delta;

  if(glfwGetKey(ctx->window, GLFW_KEY_UP) == GLFW_PRESS && onGround)
  {
    vy = -550;
  }
  if(glfwGetKey(ctx->window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    vx -= 120;
  }
  if(glfwGetKey(ctx->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    vx += 120;
  }

  x += vx * delta;
  y += min(250, max(-250, vy)) * delta;
  colliding = false;
  onGround = false;
}

void Player::render()
{
  if(!alive)
    return;
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
  glhckObjectDraw(o);
}

ew::RectCollidable::RectCollisionInformation Player::getRectCollisionInformation()
{
  return {x, y, w, h, vx, vy};
}

void Player::setRectCollisionInformation(const RectCollidable::RectCollisionInformation &newRectCollisionInformation)
{
  x = newRectCollisionInformation.x;
  y = newRectCollisionInformation.y;
  vx = newRectCollisionInformation.vx;
  vy = newRectCollisionInformation.vy;
}

void Player::handleRectCollision(RectCollidable* other)
{
  if(typeid(*other) == typeid(Block))
  {
    colliding = true;
  }
  else if(typeid(*other) == typeid(Enemy))
  {
    alive = false;
  }
  else if(typeid(*other) == typeid(Goal))
  {
    setVictorious(true);
  }

}

void Player::squishRectBlockCollision()
{
  alive = false;
}

bool Player::verticalRectBlockCollision(ew::RectBlockCollidableBlock &block, const float delta)
{
  RectCollisionInformation b =  block.getRectCollisionInformation();
  if(b.y > y)
    onGround = true;
  x += b.vx * delta;
  return RectBlockCollidableActor::verticalRectBlockCollision(block, delta);
}

bool Player::getAlive() const
{
  return alive;
}

void Player::respawn(float sx, float sy)
{
  alive = true;
  x = sx;
  y = sy;
  vx = 0;
  vy = 0;
  victorious = false;
}
bool Player::getVictorious() const
{
  return victorious;
}

void Player::setVictorious(bool value)
{
  victorious = value;
}

