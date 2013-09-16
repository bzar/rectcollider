#include "menuitems.h"
#include <sstream>

glhckText* GlhckTextMenuItem::text = nullptr;


GlhckTextMenuItem::GlhckTextMenuItem(std::string const& title, std::vector<MenuState::MenuItem*> items) : MenuItem(items),
  title(), o(nullptr)
{
  if(text == nullptr)
  {
    text = glhckTextNew(512, 512);
    glhckTextColorb(text, 255, 255, 255, 255);
  }

  setTitle(title);
}

void GlhckTextMenuItem::setTitle(const std::string& newTitle)
{
  title = newTitle;

  float x = 0;
  float y = 0;

  if(o != nullptr)
  {
    const kmAABB* aabb = glhckObjectGetAABB(o);
    x = aabb->min.x;
    y = aabb->min.y;
    glhckObjectFree(o);
  }
  int fontSize = 20;
  unsigned int font = glhckTextFontNewKakwafont(text, &fontSize);

  glhckTexture* texture = glhckTextRTT(text, font, 20, title.data(), glhckTextureDefaultParameters());
  int width, height;
  glhckTextureGetInformation(texture, nullptr, &width, &height, nullptr, nullptr, nullptr, nullptr);
  o = glhckPlaneNew(width, height);
  glhckObjectPositionf(o, x + width/2.0f, y + height/2.0f, 0);
  glhckMaterial* material = glhckMaterialNew(texture);
  glhckObjectMaterial(o, material);

  glhckTextureFree(texture);
}

void GlhckTextMenuItem::render()
{
  if(!visible)
    return;

  if(selected)
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 0, 0, 255);
  else
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 255, 255, 255);
  glhckObjectDraw(o);
}

float GlhckTextMenuItem::getHeight()
{
  const kmAABB* aabb = glhckObjectGetAABB(o);
  return aabb->max.y - aabb->min.y;
}

void GlhckTextMenuItem::setPosition(const float x, const float y)
{
  const kmAABB* aabb = glhckObjectGetAABB(o);
  float w = aabb->max.x - aabb->min.x;
  float h = aabb->max.y - aabb->min.y;
  glhckObjectPositionf(o, x + w/2, y + h/2, 0);
}

Submenu::Submenu(const std::string& title, std::vector<MenuState::MenuItem*> items) : GlhckTextMenuItem(title, items)
{

}

void Submenu::enter(MenuState* state)
{
  state->pushItem(this);
}

MenuAction::MenuAction(const std::string& title, std::function<void(MenuState *state)> action) : GlhckTextMenuItem(title),
  action(action)
{

}

void MenuAction::enter(MenuState* state)
{
  action(state);
}




BackMenuAction::BackMenuAction(const std::string& title) :
  MenuAction(title, [](MenuState* s) {s->popItem();})
{

}


MenuSelect::MenuSelect(const std::string& title,
                       const std::vector<std::string>& options,
                       std::function<void(std::string const&)> pickAction) : GlhckTextMenuItem(title),
  title(title), options(options), selectedIndex(0), pickAction(pickAction)
{
  refresh();
}

void MenuSelect::left(MenuState* state)
{
  selectedIndex = selectedIndex > 0 ? selectedIndex - 1 : options.size() - 1;
  refresh();
}

void MenuSelect::right(MenuState* state)
{
  selectedIndex = (selectedIndex + 1) % options.size();
  refresh();
}

void MenuSelect::refresh()
{
  std::ostringstream oss;
  oss << title << ": " << options.at(selectedIndex);
  setTitle(oss.str());
  pickAction(options.at(selectedIndex));
}
