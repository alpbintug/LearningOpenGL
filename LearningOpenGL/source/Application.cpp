#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderCodes
{
    std::string vertexSource;
    std::string fragmentSource;

};
static ShaderCodes ParseShaderSource(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream shaderBuffer[2]; // 0-> vertex, 1-> fragment

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType curType = ShaderType::NONE;


    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                curType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                curType = ShaderType::FRAGMENT;
            }
        }
        else
        {
            shaderBuffer[(int)curType] << line << '\n';
        }
    }
    return { shaderBuffer[0].str(),shaderBuffer[1].str() };
}
static unsigned int CompileShader(unsigned int type ,const std::string& source)
{

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id,length, &length, message);
        std::cout << "Error while compiling the " << (type == GL_VERTEX_SHADER ? "vertex": "fragment") << " shader: " << std::endl << message << std::endl;
        glDeleteShader(id);
        return 0;


    }

    return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShaderID = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertexShaderID);
    glAttachShader(program, fragmentShaderID);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return program;
}
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
        {   -0.5f, -0.5f,
            0.5f,-0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f        
        };
        unsigned int indicies[] =
        {
            0,1,2,
            2,3,0
        };

        //Creating vertex array object and binding it

        //Creating vertex thingies
        VertexArray vertexArray;
        VertexBuffer vertexBuffer(positions, sizeof(positions));
        VertexBufferLayout vertexBufferLayout;
        vertexBufferLayout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
        

        //Creating index buffer
        IndexBuffer indexBufferObject(indicies, sizeof(indicies) / sizeof(unsigned int));

        //Reading source code for shaders
        ShaderCodes source = ParseShaderSource("resources/shaders/Basic.shader");


        //Creating shaders
        unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
        glUseProgram(shader);

        //Creating uniform
        int uniformLocation = glGetUniformLocation(shader, "u_Color");
        ASSERT(uniformLocation != -1);
        float r = 1.0f, g = 0.5f, b = 0.0f;
        float rChange = 0.01f, gChange = 0.01f, bChange = 0.01f;
        glUniform4f(uniformLocation,r,g,b, 1.0f);

        vertexArray.Unbind();
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER,0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);


            GLCall(glUseProgram(shader));
            vertexArray.Bind();
            indexBufferObject.Bind();

            //Creating some kind of color shifting animation
            glUniform4f(uniformLocation, r, g, b, 1.0f);

            //Calling the macro defined function to check if error occured
            //Clearing all errors to check if the drawing function creates any errors,
            //DRAWING
            GLCall(glDrawElements(GL_TRIANGLES, sizeof(indicies)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr));
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

        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}