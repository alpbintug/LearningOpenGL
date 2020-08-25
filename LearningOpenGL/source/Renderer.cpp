#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, unsigned int line)
{
    GLenum error;
    if (error = glGetError())
    {
        std::cout << "---OpenGL error---" << std::endl << "Error code: " << error << std::endl
            << "Error occured in function " << function << std::endl
            << "File: " << file << ", Line: " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{

    shader.Bind();
    vertexArray.Bind();
    indexBuffer.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
}
