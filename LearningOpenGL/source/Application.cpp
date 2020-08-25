#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Lil Window", NULL, NULL);
    glfwSwapInterval(1); //FPS Synch
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Cannot init the glew lib" << std::endl;
        return -1;
    }
    {
        //GENERATING BUFFER AND POSITIONS FOR OUR VERTICIES OF THE TRIANGLE
        float positions[] = 
        {   -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f,-0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f        
        };
        unsigned int indicies[] =
        {
            0,1,2,
            2,3,0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        //Creating vertex array object and binding it

        //Creating vertex thingies
        VertexArray vertexArray;
        VertexBuffer vertexBuffer(positions, sizeof(positions));
        VertexBufferLayout vertexBufferLayout;
        vertexBufferLayout.Push<float>(2);
        vertexBufferLayout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
        

        //Creating index buffer
        IndexBuffer indexBufferObject(indicies, sizeof(indicies) / sizeof(unsigned int));

        //Creating and binding shader
        Shader shader("resources/shaders/Basic.shader");
        shader.Bind();
        //Color settings
        float r = 1.0f, g = 0.0f, b = 0.3f, a=1.0f;
        float rChange = 0.01f, gChange = 0.01f, bChange = 0.01f;
        //Creating uniform with colors
        shader.SetUniform4f("u_Color", r, g, b, a);

        Texture texture("resources/Textures/tex_pepe.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBufferObject.Unbind();
        shader.Unbind();

        Renderer renderer;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            //Creating some kind of color shifting animation
            shader.SetUniform4f("u_Color", r, g, b, a);

            //DRAWING
            renderer.Draw(vertexArray, indexBufferObject, shader);
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            r += rChange;
            g += gChange;
            b += bChange;

            if (r >= 1)
            {
                rChange = -0.01f;
            }
            else if (r <= 0)
            {
                rChange = 0.01f;
            }
            if (g >= 1)
            {
                gChange = -0.01f;
            }
            else if (g <= 0)
            {
                gChange = 0.01f;
            }
            if (b >= 1)
            {
                bChange = -0.01f;
            }
            else if (b <= 0)
            {
                bChange = 0.01f;
            }

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}