#pragma once

class Resource
{
public:
	Resource() {}
	virtual ~Resource() {}
};

class SamplerState : public Resource {};
class BlendState : public Resource {};
