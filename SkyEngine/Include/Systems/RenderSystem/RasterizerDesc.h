#pragma once

struct RasterizerDesc
{
	RasterizerDesc(): CullBack(true), MultisampleEnable(true), WireframeMode(false) {}
	bool CullBack;
	bool MultisampleEnable;
	bool WireframeMode;
};