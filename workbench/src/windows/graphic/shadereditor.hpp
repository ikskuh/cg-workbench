#ifndef SHADEREDITOR_HPP
#define SHADEREDITOR_HPP

#include <vector>
#include <string>
#include <memory>

#include "window.hpp"

class Shader
{
public:
	static const int MaxLength = 80 * 1024; // roughly 1024 loc
private:
	GLuint id;
	GLenum type;
	bool compiledSuccessfully;
	std::string infoLog;
	char code[MaxLength];
public:
	explicit Shader(GLenum shaderType);
	Shader(Shader const &) = delete;
	Shader(Shader &&) = delete;
	~Shader();

	char * GetCode() { return this->code; }

	char const * GetCode() const { return this->code; }

	GLuint GetObject() const { return this->id; }

	GLenum GetType() const { return this->type; }

	std::string const & GetLog() const { return this->infoLog; }

	bool IsCompiled() const { return compiledSuccessfully; }

	void Compile();
};

struct Uniform
{
	std::string name;
    GLenum type;
	Sink * sink;
	int location;
};

class ShaderEditor :
	public Window
{
	WINDOW_PREAMBLE
	friend class ShaderProgram;
private:
	GLuint program;
	ShaderProgram pgm;
	std::vector<std::unique_ptr<Shader>> shaders;
	std::vector<Uniform> uniforms;

	std::string shaderLog;

	void OnCompile() override;

	void BindUniforms();
protected:
	void OnUpdate() override;
public:
	ShaderEditor();
	virtual ~ShaderEditor();


	nlohmann::json Serialize() const override;

	void Deserialize(const nlohmann::json &value) override;
};

#endif // SHADEREDITOR_HPP
