#ifndef __PERSON_H__
#define __PERSON_H__

class Person {
public:
	Person();
	virtual ~Person();

	void set(float x, float y, float dep);

public:
	float xpos;
	float ypos;
	float depth;
	int life;
};

#endif
