#ifndef MENUITEMS_H
#define MENUITEMS_H

#include "menustate.h"
#include "glhck/glhck.h"
#include <string>
#include <functional>

class GlhckTextMenuItem : public MenuState::MenuItem
{
public:
  GlhckTextMenuItem(std::string const& title, std::vector<MenuItem*> items = std::vector<MenuItem*>());
  void setTitle(std::string const& newTitle);
  void render();
  float getHeight();
  void setPosition(const float x, const float y);

private:
  static glhckText* text;
  std::string title;
  glhckObject* o;
};

class Submenu : public GlhckTextMenuItem
{
public:
  Submenu(std::string const& title, std::vector<MenuItem*> items = std::vector<MenuItem*>());
  void enter(MenuState *state) override;
};

class MenuAction : public GlhckTextMenuItem
{
public:
  MenuAction(std::string const& title, std::function<void(MenuState *state)> action);
  void enter(MenuState *state) override;
private:
  std::function<void(MenuState *state)> action;
};

class BackMenuAction : public MenuAction
{
public:
  BackMenuAction(std::string const& title);
};

class MenuSelect : public GlhckTextMenuItem
{
public:
  MenuSelect(std::string const& title, std::vector<std::string> const& options, std::function<void(std::string const&)> pickAction);
  void left(MenuState *state) override;
  void right(MenuState *state) override;
private:
  std::string title;
  void refresh();
  unsigned int selectedIndex;
  std::vector<std::string> options;
  std::function<void(std::string const&)> pickAction;
};

#endif // MENUITEMS_H
