#ifndef CONCRETE_COMPONENT_H
#define CONCRETE_COMPONENT_H
#include "component.h"

class Concrete_Component : public Component
{
	public:
	virtual int operation(int) override;
};
#endif

