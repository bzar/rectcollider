#include "menustate.h"
#include "ew/renderphase.h"
#include "ew/updatephase.h"
#include "GLFW/glfw3.h"
#include "glfwcontext.h"
#include "ew/engine.h"
#include <algorithm>
#include <iterator>

MenuState::MenuState(std::vector<MenuItem*> items) : ew::State(),
  rootItem(new RootMenuItem(items)), itemEntities(), itemStack(), selectedIndex(0),
  pressed()
{
  phases = {new ControlPhase(this, this), new ew::UpdatePhase(this), new ew::RenderPhase(this)};

  pushItem(rootItem);

  std::list<MenuItem*> queue;
  std::copy(items.begin(), items.end(), std::inserter(queue, queue.begin()));

  while(!queue.empty())
  {
    MenuItem* i = queue.front();
    itemEntities.push_back(new MenuItemEntity(this, i));
    std::copy(i->getChildren().begin(), i->getChildren().end(), std::inserter(queue, queue.end()));
    queue.pop_front();
  }

  updateLayout();
  selectedItem()->setSelected(true);
}

void MenuState::controlPhase(const float delta)
{
  if(keyPress(GLFW_KEY_UP))
  {
    prevItem();
  }
  if(keyPress(GLFW_KEY_DOWN))
  {
    nextItem();
  }
  if(keyPress(GLFW_KEY_LEFT))
  {
    selectedItem()->left(this);
  }
  if(keyPress(GLFW_KEY_RIGHT))
  {
    selectedItem()->right(this);
  }
  if(keyPress(GLFW_KEY_ENTER))
  {
    selectedItem()->enter(this);
  }
}

void MenuState::pushItem(MenuState::MenuItem* item)
{
  if(item->getChildren().empty())
    return;

  for(MenuItem* i : itemStack.front()->getChildren())
  {
    i->setVisible(false);
  }

  itemStack.push_front(item);

  for(MenuItem* i : itemStack.front()->getChildren())
  {
    i->setVisible(true);
  }

  selectedIndex = 0;
  updateLayout();
}

void MenuState::popItem()
{
  if(itemStack.size() < 2)
    return;

  for(MenuItem* i : itemStack.front()->getChildren())
  {
    i->setVisible(false);
  }

  itemStack.pop_front();

  for(MenuItem* i : itemStack.front()->getChildren())
  {
    i->setVisible(true);
  }

  selectedIndex = 0;
  updateLayout();
}

void MenuState::nextItem()
{
  selectedItem()->setSelected(false);
  selectedIndex = (selectedIndex + 1) % itemStack.front()->getChildren().size();
  selectedItem()->setSelected(true);
}

void MenuState::prevItem()
{
  selectedItem()->setSelected(false);
  selectedIndex = selectedIndex > 0 ? selectedIndex - 1 : itemStack.front()->getChildren().size() - 1;
  selectedItem()->setSelected(true);
}

bool MenuState::keyPress(int key)
{
  GLFWContext* ctx = engine->singletons.get<GLFWContext>();

  auto it = pressed.find(key);
  if(glfwGetKey(ctx->window, key) == GLFW_PRESS)
  {
    if(it == pressed.end())
    {
      pressed.insert(key);
      return true;
    }
  }
  else if(it != pressed.end())
  {
    pressed.erase(it);
  }

  return false;
}

MenuState::MenuItem* MenuState::selectedItem()
{
  return itemStack.front()->getChildren().at(selectedIndex);
}

void MenuState::updateLayout()
{
  float y = 0;
  for(MenuItem* i : itemStack.front()->getChildren())
  {
    i->setPosition(0, y);
    y += i->getHeight();
  }
}

MenuState::MenuItem::MenuItem(std::vector<MenuItem*> children) : visible(false), selected(false), children(children)
{

}

std::vector<MenuState::MenuItem*>& MenuState::MenuItem::getChildren()
{
  return children;
}

void MenuState::MenuItem::setVisible(bool value)
{
  visible = value;
}

void MenuState::MenuItem::setSelected(bool value)
{
  selected = value;
}


MenuState::RootMenuItem::RootMenuItem(std::vector<MenuItem*> items) : MenuItem()
{
  children = items;
}


MenuState::MenuItemEntity::MenuItemEntity(MenuState* state, MenuState::MenuItem* item) :
  item(item), roles(this, state)
{
}

void MenuState::MenuItemEntity::render()
{
  item->render();
}
