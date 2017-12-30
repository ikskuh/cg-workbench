#pragma once

#include "window.hpp"

class MatrixTransforms :
	public Window
{
protected:
	glm::mat4 value;

	void OnUpdate() override;

	MatrixTransforms(std::string const & title);
};

class MatrixMult :
	public MatrixTransforms
{
private:
	Sink * lhs;
	Sink * rhs;
protected:
	void OnRender() override;
public:
	MatrixMult();

	TRIVIAL_SERIALIZE("matrix-mult")
};

class MatrixTranslate :
	public MatrixTransforms
{
private:
	Sink * input;
	Sink * offset;
protected:
	void OnRender() override;
public:
	MatrixTranslate();

	TRIVIAL_SERIALIZE("matrix-translate")
};

class MatrixRotate :
	public MatrixTransforms
{
private:
	Sink * input;
	Sink * axis;
	Sink * angle;
protected:
	void OnRender() override;
public:
	MatrixRotate();

	TRIVIAL_SERIALIZE("matrix-rotate")
};

class MatrixScale :
	public MatrixTransforms
{
private:
	Sink * input;
	Sink * scaling;
protected:
	void OnRender() override;
public:
	MatrixScale();

	TRIVIAL_SERIALIZE("matrix-scale")
};

class MatrixLookAt :
	public MatrixTransforms
{
private:
	Sink * input;
	Sink * eye;
	Sink * center;
	Sink * up;
protected:
	void OnRender() override;
public:
	MatrixLookAt();

	TRIVIAL_SERIALIZE("matrix-lookat")
};

class MatrixPerspective :
	public MatrixTransforms
{
private:
	Sink * input;
	Sink * aspect;
	Sink * fov;
	Sink * znear;
	Sink * zfar;
protected:
	void OnRender() override;
public:
	MatrixPerspective();

	TRIVIAL_SERIALIZE("matrix-perspective")
};
