#pragma once
#include "ResourcesImpl.h"
#include "common.h"
#include <memory>
#include "D3D11Device.h"

using namespace std;

class SKYENGINEDLL Application
{
public:
	Application();
	~Application();
	bool init(HWND window, int width, int height);
	void tick();
private:
	void render();
	shared_ptr<ViewportImpl> viewPort;
	shared_ptr<D3D11Device> mDevice;
};

