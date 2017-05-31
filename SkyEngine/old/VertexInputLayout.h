#pragma once
#include <d3d11.h>

namespace sky
{
	enum ELEMENT_FORMAT
	{
		FLOAT2 = 0,
		FLOAT3 = 1,
		FLOAT4 = 2
	};

	struct VertexLayoutDesc
	{
		char* SemanticName;
		unsigned int SemanticIndex;
		ELEMENT_FORMAT Format;
		unsigned int InputSlot;
		bool ForInstance;
	};

	class VertexInputLayout
	{
	public:
		VertexInputLayout();
		~VertexInputLayout();
		void setData(ID3D11InputLayout *data);
		ID3D11InputLayout* getData() const;
	private:
		ID3D11InputLayout* m_layout;
	};
}



