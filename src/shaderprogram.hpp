#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>

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

	GLuint GetProgram() const;

    // HACK: const here is a hack so we can actually change some shader variables!

	void BindUniforms() const;

	void Use() const;

    void SetTransform(glm::mat4 const & t) const;
};
