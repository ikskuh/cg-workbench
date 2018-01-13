#include "shaderprogram.hpp"
#include "windows/graphic/shadereditor.hpp"

#include <algorithm>

ShaderProgram::ShaderProgram() : edit(nullptr) { }

ShaderProgram::ShaderProgram(ShaderEditor * edit) : edit(edit) { }

ShaderProgram::~ShaderProgram() { }

GLuint ShaderProgram::GetProgram() const
{
	if(this->edit)
		return this->edit->program;
	else
		return 0;
}

void ShaderProgram::BindUniforms() const
{
	if(this->edit)
		this->edit->BindUniforms();
}

void ShaderProgram::Use() const
{
	glUseProgram(this->GetProgram());
	this->BindUniforms();
}

void ShaderProgram::SetTransform(glm::mat4 const & t) const
{
    if(this->edit == nullptr)
        return;
    auto it = std::find_if(
        this->edit->uniforms.begin(),
        this->edit->uniforms.end(),
        [](Uniform const & u) -> bool
        {
            return (u.name == "transform") && (u.type == GL_FLOAT_MAT4);
        });
    if(it != this->edit->uniforms.end())
    {
        glProgramUniformMatrix4fv(
            this->edit->program,
            it->location,
            1,
            GL_FALSE,
            &t[0][0]);
    }
}
