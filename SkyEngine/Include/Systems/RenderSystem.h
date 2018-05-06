#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Systems\ISystem.h"
#include <Windows.h>
#include <memory>
#include "Systems\RenderSystem\IDeviceImpl.h"
#include "Systems\RenderSystem\ViewPort.h"
#include "Systems\RenderSystem\MeshFactory.h"
#include "Systems\RenderSystem\ShaderFactory.h"

#define main_buffer_size_multiplier			1.1f

class RenderSystem : public ISystem
{
public:
	RenderSystem();
	~RenderSystem();
	void Update(double time) override;
	void Start() override;
	void Stop() override;
	void Init(HWND hWnd, int width, int height);
	bool RegisterEntity(Entity *entity) override;
	IDeviceImpl* GetDevice() const;
	MeshFactory* GetMeshFactory() const;
	ShaderFactory* GetShaderFactory() const;
private:
	void RenderToBackBuffer();

	std::shared_ptr<IDeviceImpl> device;
	std::unique_ptr<ViewPort> viewPort;
	std::unique_ptr<MeshFactory> meshFactory;
	std::unique_ptr<ShaderFactory> shaderFactory;

	Texture2D* drawingSurface;
	Texture2D* drawingSurfaceResolved;
};
