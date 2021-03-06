#include "GLFW/glfw3.h"
#include "glhck/glhck.h"
#include "ew/engine.h"
#include "ew/interceptor.h"
#include "glfwcontext.h"

#include "menustate.h"
#include "menuitems.h"

#include "gamestate.h"
#include "player.h"
#include "block.h"

#include <iostream>
#include <algorithm>

void windowCloseCallback(GLFWwindow* window);
void gameloop(GLFWwindow* window);

int const WIDTH = 800;
int const HEIGHT = 480;

class GlhckGLFWInterceptor : public ew::Interceptor<ew::Engine>
{
  void before(ew::Engine *t)
  {
    glfwPollEvents();
    glhckRenderClear(GLHCK_DEPTH_BUFFER | GLHCK_COLOR_BUFFER);
  }

  void after(ew::Engine *engine)
  {
    GLFWContext* ctx = engine->singletons.get<GLFWContext>();
    glhckRender();
    glfwSwapBuffers(ctx->window);
  }
};

int main(int argc, char** argv)
{
  if (!glfwInit())
    return EXIT_FAILURE;

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "colliderect", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if(!window)
  {
    return EXIT_FAILURE;
  }

  glfwSwapInterval(1);
  glfwSetWindowCloseCallback(window, windowCloseCallback);

  if(!glhckContextCreate(argc, argv))
  {
    std::cerr << "GLHCK initialization error" << std::endl;
    return EXIT_FAILURE;
  }

  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_AUTO))
  {
    std::cerr << "GLHCK display create error" << std::endl;
    return EXIT_FAILURE;
  }


  gameloop(window);


  glhckContextTerminate();
  glfwTerminate();
}

void windowCloseCallback(GLFWwindow* window)
{
  ew::Engine* engine = static_cast<ew::Engine*>(glfwGetWindowUserPointer(window));
  engine->quit();
}

void gameloop(GLFWwindow* window)
{
  ew::Engine engine;
  glfwSetWindowUserPointer(window, &engine);

  GLFWContext glfwContext = {window};
  engine.singletons.set<GLFWContext>(&glfwContext);

  GlhckGLFWInterceptor glhckGLFWInterceptor;
  engine.addInterceptor(&glhckGLFWInterceptor);

  std::vector<std::string> levels = {"levels/001.qmlon", "levels/002.qmlon", "levels/003.qmlon", "levels/004.qmlon",
                                     "levels/005.qmlon", "levels/006.qmlon", "levels/007.qmlon", "levels/008.qmlon",
                                     "levels/009.qmlon", "levels/010.qmlon", "levels/011.qmlon", "levels/012.qmlon",
                                     "levels/013.qmlon"};
  GameState gameState(levels);
  MenuState menuState({
                        new MenuSelect("Level", levels, [&gameState](std::string const& level) {gameState.setLevel(level);}),
                        new MenuAction("Start", [](MenuState* s) { s->engine->setState(1);}),
                        new Submenu("Title 1", {
                                      new Submenu("Title 1-1"),
                                      new Submenu("Title 1-2"),
                                      new MenuSelect("Pick one", {"A", "B", "42", "Robert"}, [](std::string const&) {}),
                                      new BackMenuAction("Back")
                                    }),
                      });



  engine.addState(0, &menuState);
  engine.addState(1, &gameState);
  engine.setState(0);

  engine.run();

  double t0 = glfwGetTime();
  while(engine.isRunning())
  {
    double t = glfwGetTime();
    double dt = t - t0;
    t0 = t;
    engine.advance(dt);
  }
}
