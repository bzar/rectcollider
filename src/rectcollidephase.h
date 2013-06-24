#ifndef RECTCOLLIDEPHASE_H
#define RECTCOLLIDEPHASE_H

#include "ew/phase.h"
#include "ew/state.h"

class RectCollidePhase : public ew::Phase
{
public:
  RectCollidePhase(ew::State* state);

  void execute(const float delta);
};

#endif // RECTCOLLIDEPHASE_H
