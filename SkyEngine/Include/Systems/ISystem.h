#pragma once

class ISystem
{
public:
	virtual ~ISystem(){}
	virtual void Update(double time) = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
};
