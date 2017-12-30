#include "shaderprogram.hpp"
#include "windows/graphic/shadereditor.hpp"

ShaderProgram::ShaderProgram() : edit(nullptr) { }

ShaderProgram::ShaderProgram(ShaderEditor * edit) : edit(edit) { }

ShaderProgram::~ShaderProgram() { }

GLuint ShaderProgram::GetProgram()
{
	if(this->edit)
		return this->edit->program;
	else
		return 0;
}

void ShaderProgram::BindUniforms()
{
	if(this->edit)
		this->edit->BindUniforms();
}

void ShaderProgram::Use()
{
	glUseProgram(this->GetProgram());
	this->BindUniforms();
}
