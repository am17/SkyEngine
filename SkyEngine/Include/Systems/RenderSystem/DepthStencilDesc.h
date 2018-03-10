#pragma once

struct DepthStencilDesc
{
	DepthStencilDesc(): DepthEnable(true), StencilEnable(false) {}
	bool DepthEnable;
	bool StencilEnable;
};
