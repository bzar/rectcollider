#include "player.h"

#include "ew/engine.h"
#include "GLFW/glfw3.h"
#include "glfwcontext.h"
#include "rectblockcollidableblock.h"

#include <iostream>

Player::Player(float x, float y, float w, float h, ew::State* state) :
  ew::Updatable(), ew::Renderable(), RectBlockCollidableActor(),
  o(nullptr), x(x), y(y), w(w), h(h), vx(0), vy(0),
  state(state), roles(this, state)
{
  o = glhckPlaneNew(w, h);
  glhckMaterial* mat = glhckMaterialNew(nullptr);
  glhckMaterialDiffuseb(mat, 255, 255, 255, 255);
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
  if(colliding)
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 0, 0, 255);
  else
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 255, 255, 255);

  GLFWContext* ctx = state->engine->singletons.get<GLFWContext>();

  vx = 0;
  vy = min(vy + delta * 3000, 500);

  if(glfwGetKey(ctx->window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    vy = -500;
  }
  if(glfwGetKey(ctx->window, GLFW_KEY_LEFT) == GLFW_PRESS)
  {
    vx -= 200;
  }
  if(glfwGetKey(ctx->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
  {
    vx += 200;
  }

  x += vx * delta;
  y += vy * delta;
  colliding = false;
}

void Player::render()
{
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
  glhckObjectDraw(o);
}

RectCollidable::RectCollisionInformation Player::getRectCollisionInformation()
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

void Player::handleRectCollision(RectCollidable *)
{
  colliding = true;
}

void Player::squishRectBlockCollision()
{
  std::cout << "SQUISH!" << std::endl;
}

bool Player::verticalRectBlockCollision(RectBlockCollidableBlock &block, const float delta)
{
  x += block.getRectCollisionInformation().vx * delta;
  return RectBlockCollidableActor::verticalRectBlockCollision(block, delta);
}
