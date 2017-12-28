#include "matrixtransforms.hpp"
#include <glm/gtx/transform.hpp>

MatrixTransforms::MatrixTransforms(std::string const & title) :
    Window(title, ImGuiWindowFlags_AlwaysAutoResize)
{
	this->AddSource(new Source(CgDataType::UniformMat4, "Result", &this->value));
}

void MatrixTransforms::OnUpdate()
{
	ImGui::TextUnformatted(this->GetTitle().c_str());
}

#define SINK(_Name) new Sink(CgDataType::UniformMat4, _Name)
#define MAT(_Val) (_Val)->GetObject<CgDataType::UniformMat4>()
#define VEC(_Val) (_Val)->GetObject<CgDataType::UniformVec3>()
#define VEC_DEF(_Val, _Default) ((_Val)->HasSourceConnected() ? VEC(_Val) : (_Default))
#define FLT(_Val) (_Val)->GetObject<CgDataType::UniformFloat>()
#define FLT_DEF(_Val, _Default) ((_Val)->HasSourceConnected() ? FLT(_Val) : (_Default))

MatrixMult::MatrixMult() :
    MatrixTransforms("Matrix Mult.")
{
	this->AddSink(this->lhs = SINK("LHS"));
	this->AddSink(this->rhs = SINK("RHS"));
}

void MatrixMult::OnRender()
{
	this->value = MAT(lhs) * MAT(rhs);
}

MatrixTranslate::MatrixTranslate() :
	MatrixTransforms("Matrix Translate")
{
	this->AddSink(this->input = new Sink(CgDataType::UniformMat4, "Src"));
	this->AddSink(this->offset = new Sink(CgDataType::UniformVec3, "Offset"));
}

void MatrixTranslate::OnRender()
{
	this->value = MAT(input) * glm::translate(glm::mat4(), VEC(offset));
}


MatrixRotate::MatrixRotate() :
	MatrixTransforms("Matrix Rotate")
{
	this->AddSink(this->input = new Sink(CgDataType::UniformMat4, "Src"));
	this->AddSink(this->axis = new Sink(CgDataType::UniformVec3, "Axis"));
	this->AddSink(this->angle = new Sink(CgDataType::UniformFloat, "Angle"));
}

void MatrixRotate::OnRender()
{
	this->value = MAT(input) * glm::rotate(glm::mat4(), FLT(angle), VEC_DEF(axis, glm::vec3(0,1,0)));
}


MatrixScale::MatrixScale() :
	MatrixTransforms("Matrix Scale")
{
	this->AddSink(this->input = new Sink(CgDataType::UniformMat4, "Src"));
	this->AddSink(this->scaling = new Sink(CgDataType::UniformVec3, "Scale"));
}

void MatrixScale::OnRender()
{
	this->value = MAT(input) * glm::scale(glm::mat4(), VEC_DEF(scaling, glm::vec3(1,1,1)));
}

MatrixLookAt::MatrixLookAt() :
	MatrixTransforms("Matrix LookAt")
{
	this->AddSink(this->input = new Sink(CgDataType::UniformMat4, "Src"));
	this->AddSink(this->eye = new Sink(CgDataType::UniformVec3, "Eye"));
	this->AddSink(this->center = new Sink(CgDataType::UniformVec3, "Center"));
	this->AddSink(this->up = new Sink(CgDataType::UniformVec3, "Up"));
}

void MatrixLookAt::OnRender()
{
	this->value = MAT(input) * glm::lookAt(
		VEC_DEF(eye, glm::vec3(0,0,0)),
		VEC_DEF(center, glm::vec3(0,0,-1)),
		VEC_DEF(up, glm::vec3(0,1,0)));
}


MatrixPerspective::MatrixPerspective() :
	MatrixTransforms("Matrix Perspective")
{
	this->AddSink(this->input = new Sink(CgDataType::UniformMat4, "Src"));
	this->AddSink(this->aspect = new Sink(CgDataType::UniformFloat, "Aspect"));
	this->AddSink(this->fov = new Sink(CgDataType::UniformFloat, "FoV"));
	this->AddSink(this->znear = new Sink(CgDataType::UniformFloat, "ZNear"));
	this->AddSink(this->zfar = new Sink(CgDataType::UniformFloat, "ZFar"));
}

void MatrixPerspective::OnRender()
{
	this->value = MAT(input) * glm::perspectiveFov(
		std::max<float>(glm::radians(1.0), FLT_DEF(fov, glm::radians(60.0f))),
		std::max<float>(0.01, FLT_DEF(aspect, 1.0f)), 1.0f,
		FLT_DEF(znear, 0.01f), FLT_DEF(zfar, 10000.0f));
}
