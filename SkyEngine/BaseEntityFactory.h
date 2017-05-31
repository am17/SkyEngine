#pragma once
#include "Entity.h"
#include "Device.h"
#include "common.h"
#include "ShaderFactory.h"

class BaseEntityFactory
{
public:
	SKYENGINEDLL BaseEntityFactory(Device* device);
	SKYENGINEDLL ~BaseEntityFactory();
	SKYENGINEDLL Entity* createEntity();
	SKYENGINEDLL Entity* createCubeEntity();
	SKYENGINEDLL Entity* createTeapotEntity();
	SKYENGINEDLL Entity* createSphereEntity();
	SKYENGINEDLL Entity* createBoxEntity();
	SKYENGINEDLL Entity* createGeoSphereEntity();
	SKYENGINEDLL Entity* createCylinderEntity();
	SKYENGINEDLL Entity* createConeEntity();
	SKYENGINEDLL Entity* createTorusEntity();
	SKYENGINEDLL Entity* createTetrahedronEntity();
	SKYENGINEDLL Entity* createOctahedronEntity();
	SKYENGINEDLL Entity* createDodecahedronEntity();
	SKYENGINEDLL Entity* createIcosahedronEntity();
	SKYENGINEDLL Entity* createQuadEntity();
private:
	Entity* initBaseEntity(const void *pVBData, unsigned int vbElementsCount, const void *pIBData, unsigned int ibElementsCount);
	Device* _pDevice;
	ShaderFactory *_pShaderFactory;
};

