#ifndef EXIT_H
#define EXIT_H

#include <string>
#include "ew/state.h"
#include "ew/rectcollidable.h"
#include "ew/rolemanager.h"

class Exit : public ew::RectCollidable
{
public:
  Exit(std::string const& destination, int const id, float x, float y, float w, float h, ew::State* state);
  RectCollisionInformation getRectCollisionInformation() override;
  void setRectCollisionInformation(const RectCollisionInformation &newRectCollisionInformation) override;

  std::string getDestination() const;
  void setDestination(const std::string& value);

  int getId() const;
  void setId(int value);

private:
  std::string destination;
  int id;
  float x;
  float y;
  float w;
  float h;

  ew::State* state;
  ew::RoleManager<Exit, ew::RectCollidable> roles;

};

#endif // EXIT_H
