#pragma once
#include <vector>
#include <GL/glew.h>
struct VertexBufferLayoutElement
{
	unsigned int type;
	unsigned int count;
	bool normalized;
};
class VertexBufferLayout
{
public:
	VertexBufferLayout();

	template<typename T>
	void Push(int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(int count)
	{
		m_Elements.push_back({GL_FLOAT, count, false});
	}

	template<>
	void Push<float>(int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, false });
	}
private:
	std::vector<VertexBufferLayoutElement> m_Elements;
};
