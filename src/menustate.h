#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "ew/state.h"
#include "ew/customphase.h"
#include "ew/renderable.h"
#include "ew/rolemanager.h"

#include <list>
#include <vector>
#include <unordered_set>

class MenuState : public ew::State
{
public:
  class MenuItem
  {
  public:
    MenuItem(std::vector<MenuItem*> children = std::vector<MenuItem*>());
    virtual void right(MenuState* state) {}
    virtual void left(MenuState* state) {}
    virtual void enter(MenuState* state) {}
    virtual void render() {}
    virtual float getHeight() {return 0; }
    virtual void setPosition(float const x, float const y) {}
    void setVisible(bool value);
    void setSelected(bool value);
    std::vector<MenuItem*>& getChildren();

  protected:
    bool visible;
    bool selected;
    std::vector<MenuItem*> children;
  };

  MenuState(std::vector<MenuItem*> items);
  void controlPhase(const float delta);

  void pushItem(MenuItem* item);
  void popItem();
  void nextItem();
  void prevItem();

private:
  class MenuItemEntity: public ew::Renderable
  {
  public:
    MenuItemEntity(MenuState* state, MenuItem* item);
    void render() override;
  private:
    MenuItem* item;
    ew::RoleManager<MenuItemEntity, ew::Renderable> roles;
  };

  class RootMenuItem : public MenuItem
  {
  public:
    RootMenuItem(std::vector<MenuItem*> items);
  };

  typedef ew::CustomPhase<MenuState, &MenuState::controlPhase> ControlPhase;

  bool keyPress(int key);

  MenuItem* selectedItem();
  void updateLayout();
  std::list<MenuItemEntity*> itemEntities;
  std::list<MenuItem*> itemStack;
  MenuItem* rootItem;
  unsigned int selectedIndex;
  std::unordered_set<int> pressed;
};

#endif // MENUSTATE_H
