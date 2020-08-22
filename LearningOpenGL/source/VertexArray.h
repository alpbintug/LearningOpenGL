#pragma once
#include "VertexBuffer.h"
class VertexArray
{
private:

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vertexBuffer,const VertexBufferLayout& vertexBufferLayout);
};