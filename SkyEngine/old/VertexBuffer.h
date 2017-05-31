#pragma once
#include "IAPIBuffer.h"
#include <vector>
#include "VertexFormat.h"
#include "VertexInputLayout.h"

using namespace std;
using namespace render;

namespace sky
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();
		IAPIBuffer *getAPIVertexBuffer() const;
	protected:
		IAPIBuffer *m_APIVertexBuffer;
	};

	class StaticMeshVertexBuffer : public VertexBuffer
	{
	public:
		StaticMeshVertexBuffer();
		~StaticMeshVertexBuffer();
		void init(const vector<StaticMeshVertex> & vertices);
		unsigned int getStride() const;
		size_t getVerticesCount() const;
		const void* getRawData() const;
	private:
		void* rawData;
		unsigned int m_stride;
		size_t m_numVertices;
	};

	class ColorVertexBuffer : public VertexBuffer
	{
	public:
		ColorVertexBuffer();
		~ColorVertexBuffer();
		void init(const vector<ColorVertex> & vertices);
		unsigned int getStride() const;
		size_t getVerticesCount() const;
		const void* getRawData() const;
	private:
		void* rawData;
		unsigned int m_stride;
		size_t m_numVertices;
	};
}

