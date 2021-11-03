#pragma once
#ifndef __EXAMPLEOBJECT__
#define __EXAMPLEOBJECT__
#include "DisplayObject.h"

class ExampleObject final : public DisplayObject
{
public:
	// constructors
	ExampleObject();

	// destructor
	virtual ~ExampleObject();

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
private:

};

#endif