#pragma once
#include "Resource.h"

class ViewPort : public Resource
{
public:
	ViewPort() {}
	~ViewPort() {}
	virtual void Present() {}
	virtual void Clear() {}
private:

};