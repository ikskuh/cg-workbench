#include "renderpassnode.hpp"

#include "windowregistry.hpp"
REGISTER_WINDOW_CLASS(RenderPassNode, Menu::Graphic, "graphic-render-pass", "Render Pass")

RenderPassNode::RenderPassNode() :
    Window("Render Pass", 0),
    priority(0),
	pass()
{
    this->geom = this->AddSink<CgDataType::Geometry>("Geometry");
    this->shader = this->AddSink<CgDataType::Shader>("Shader");
    this->transform = this->AddSink<CgDataType::UniformMat4>("Transform");

    this->AddSource<CgDataType::RenderPass>("Drawcall", &this->pass);
}

void RenderPassNode::OnRender()
{
    this->pass.geometry = &this->geom->GetObject<CgDataType::Geometry>();
    this->pass.shader = &this->shader->GetObject<CgDataType::Shader>();
    this->pass.transform = this->transform->GetObject<CgDataType::UniformMat4>();
}

void RenderPassNode::OnUpdate()
{
    ImGui::DragInt("Priority", &this->priority, 0.1);
}

WINDOW_SERIALIZE_IMPL(RenderPassNode)
{
    return {
        { "priority", this->priority }
    };
}

WINDOW_DESERIALIZE_IMPL(RenderPassNode)
{
    this->priority = data.value("priority", 0);
}
