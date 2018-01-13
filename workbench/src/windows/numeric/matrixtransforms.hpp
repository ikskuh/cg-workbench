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
	WINDOW_PREAMBLE
private:
	Sink * lhs;
	Sink * rhs;
protected:
	void OnRender() override;
public:
	MatrixMult();
};

class MatrixTranslate :
	public MatrixTransforms
{
	WINDOW_PREAMBLE
private:
	Sink * input;
	Sink * offset;
protected:
	void OnRender() override;
public:
	MatrixTranslate();
};

class MatrixRotate :
	public MatrixTransforms
{
	WINDOW_PREAMBLE
private:
	Sink * input;
	Sink * axis;
	Sink * angle;
protected:
	void OnRender() override;
public:
	MatrixRotate();
};

class MatrixScale :
	public MatrixTransforms
{
	WINDOW_PREAMBLE
private:
	Sink * input;
	Sink * scaling;
protected:
	void OnRender() override;
public:
	MatrixScale();
};

class MatrixLookAt :
	public MatrixTransforms
{
	WINDOW_PREAMBLE
private:
	Sink * input;
	Sink * eye;
	Sink * center;
	Sink * up;
protected:
	void OnRender() override;
public:
	MatrixLookAt();
};

class MatrixPerspective :
	public MatrixTransforms
{
	WINDOW_PREAMBLE
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
};
