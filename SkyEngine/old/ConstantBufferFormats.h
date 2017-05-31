#pragma once
#include <DirectXMath.h>
#include "IMaterial.h"

using namespace DirectX;

 namespace sky
 {
	 struct CBPerObject
	 {
		 XMFLOAT4X4 WVP;
		 XMFLOAT4X4 world;
	 };

	 struct SDirectionalLight
	 {
		 XMFLOAT4 ambientColor;
		 XMFLOAT4 diffuseColor;
		 XMFLOAT3 lightDirection;
		 float specularPower;
		 XMFLOAT4 specularColor;
		 XMFLOAT3 halfWayVector;
		 float textureTranslation;
	 };

	 struct SPointLight
	 {
		 XMFLOAT4 ambientColor;
		 XMFLOAT4 diffuseColor;
		 XMFLOAT3 position;
		 float specularPower;
		 XMFLOAT4 specularColor;
		 XMFLOAT3 halfWayVector;
		 float range;
		 XMFLOAT3 attenuation;
		 float padding;
	 };

	 struct CBPerFrame
	 {
		 SDirectionalLight directionalLight;
		 //SPointLight pointLight;
	 };

	 struct SCameraBuffer
	 {
		 XMFLOAT3 cameraPosition;
		 float padding;
		 XMFLOAT4X4 reflectionMatrix;
		 XMFLOAT4X4 projection;
	 };
 }