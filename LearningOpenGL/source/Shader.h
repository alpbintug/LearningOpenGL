#pragma once
#include <string>

struct ShaderCodes
{
	std::string vertexSource;
	std::string fragmentSource;

};
class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	//caching for uniforms
	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderCodes ParseShaderSource(const std::string& filepath);
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};
