#pragma once
#include "IAPIBuffer.h"
#include "IShader.h"
#include "D3D11PipelineState.h"
#include "ConstantBufferFormats.h"
#include "Behavior.h"

#include <DirectXMath.h>

using namespace DirectX;
using namespace sky;

namespace scene
{
	class IEntity
	{
	public:
		virtual ~IEntity() {}
		virtual void update(const XMFLOAT4X4 &viewMatrix, const XMFLOAT4X4 &projectMatrix) = 0;
		virtual void setRotation(float angleX, float angleY, float angleZ) = 0;
		virtual void setPosition(float x, float y, float z) = 0;
		virtual void setScale(float scaleX, float scaleY, float scaleZ) = 0;
		virtual void setBehavior(Behavior* behavior) = 0;
	};
}