#pragma once
#include "VertexElement.h"

class Resource
{
public:
	Resource() {}
	virtual ~Resource() {}
};

class SamplerState : public Resource {};
class BlendState : public Resource {};
class VertexDeclaration : public Resource {};
