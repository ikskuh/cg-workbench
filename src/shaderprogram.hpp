#pragma once

#include <GL/gl3w.h>

class ShaderEditor;

//! OpenGL Shader Object
class ShaderProgram
{
private:
	ShaderEditor * edit;
public:
	ShaderProgram();
	explicit ShaderProgram(ShaderEditor * edit);
	~ShaderProgram();

	GLuint GetProgram();

	void BindUniforms();

	void Use();
};