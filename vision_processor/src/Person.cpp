#include "vision_processor/Person.h"

Person::Person() {}
Person::~Person() {}

void Person::set(float x, float y, float dep)
{
	xpos = x;
	ypos = y;
	depth = dep;
}
