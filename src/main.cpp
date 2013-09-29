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

#include <boost/filesystem.hpp>

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

  glhckLogColor(0);
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



  boost::filesystem::path levelPath("levels");
  std::vector<std::string> levels;

  for(boost::filesystem::directory_iterator i(levelPath); i != boost::filesystem::directory_iterator(); ++i)
  {
    if(!boost::filesystem::is_directory(i->status()))
    {
      boost::filesystem::path file = i->path().leaf();
      if(file.has_extension() && file.extension() == ".tmx")
      {
        levels.push_back(file.generic_string());
      }
    }
  }

  std::sort(levels.begin(), levels.end());

  std::string level = levels.front();
  GameState gameState("levels", level);
  MenuState menuState({
                        new MenuAction("Start", [&gameState, &level](MenuState* s) { std::cout << level << std::endl;gameState.loadLevel(level); s->engine->setState(1);}),
                        new MenuSelect("Level", levels, [&level](std::string const& l) {level = l;}),
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
    engine.advance(1.0f/60.0f);
    //engine.advance(dt);
  }
}
