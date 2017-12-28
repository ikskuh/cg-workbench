#include "vectoradapter.hpp"

VectorSplitter::VectorSplitter() :
    Window("Vector Splitter", ImGuiWindowFlags_AlwaysAutoResize),
    output(),
    components(4)
{
	this->AddSink(this->input = new Sink(CgDataType::UniformVec4, "Input"));

	this->AddSource(new Source(CgDataType::UniformFloat, "X", &this->output.x));
	this->AddSource(new Source(CgDataType::UniformFloat, "Y", &this->output.y));
	this->AddSource(new Source(CgDataType::UniformFloat, "Z", &this->output.z));
	this->AddSource(new Source(CgDataType::UniformFloat, "W", &this->output.w));
}

void VectorSplitter::SetComponents(int num)
{
	if(this->components == num)
		return;
	this->components = num;
	this->RemoveSink(this->input);
	switch(num)
	{
	case 2:
		this->input = new Sink(CgDataType::UniformVec2, "Input");
		break;
	case 3: this->input = new Sink(CgDataType::UniformVec3, "Input"); break;
	case 4: this->input = new Sink(CgDataType::UniformVec4, "Input"); break;
	default: abort();
	}

	while(this->GetSourceCount() > num)
		this->RemoveSource(this->GetSource(num));

	if(num > 0 && this->GetSourceCount() < 1)
		this->AddSource(new Source(CgDataType::UniformFloat, "X", &this->output.x));
	if(num > 1 && this->GetSourceCount() < 2)
		this->AddSource(new Source(CgDataType::UniformFloat, "Y", &this->output.y));
	if(num > 2 && this->GetSourceCount() < 3)
		this->AddSource(new Source(CgDataType::UniformFloat, "Z", &this->output.z));
	if(num > 3 && this->GetSourceCount() < 4)
		this->AddSource(new Source(CgDataType::UniformFloat, "W", &this->output.w));

	this->AddSink(this->input);
}

void VectorSplitter::OnRender()
{
	switch(this->components)
	{
		case 2:
			this->output = glm::vec4(this->input->GetObject<CgDataType::UniformVec2>(), 0, 0);
			break;
		case 3:
			this->output = glm::vec4(this->input->GetObject<CgDataType::UniformVec3>(), 0);
			break;
		case 4:
			this->output = this->input->GetObject<CgDataType::UniformVec4>();
			break;
		default: abort();
	}
}

void VectorSplitter::OnUpdate()
{
	int com = this->components;
	if(ImGui::RadioButton("vec2", (com == 2))) com = 2;
	if(ImGui::RadioButton("vec3", (com == 3))) com = 3;
	if(ImGui::RadioButton("vec4", (com == 4))) com = 4;
	this->SetComponents(com);
}

nlohmann::json VectorSplitter::Serialize() const
{
	return {
		{ "type", "vector-splitter" },
		{ "components", this->components }
	};
}

void VectorSplitter::Deserialize(const nlohmann::json &value)
{
	this->SetComponents(value["components"]);
}


VectorMerger::VectorMerger() :
    Window("Vector Merger"),
    output()
{
	this->AddSink(this->inputX = new Sink(CgDataType::UniformFloat, "X"));
	this->AddSink(this->inputY = new Sink(CgDataType::UniformFloat, "Y"));
	this->AddSink(this->inputZ = new Sink(CgDataType::UniformFloat, "Z"));
	this->AddSink(this->inputW = new Sink(CgDataType::UniformFloat, "W"));

	this->AddSource(new Source(CgDataType::UniformVec2, "Vec2", &this->output.x));
	this->AddSource(new Source(CgDataType::UniformVec3, "Vec3", &this->output.x));
	this->AddSource(new Source(CgDataType::UniformVec4, "Vec4", &this->output.x));
}

void VectorMerger::OnRender()
{
	this->output.x = this->inputX->GetObject<CgDataType::UniformFloat>();
	this->output.y = this->inputY->GetObject<CgDataType::UniformFloat>();
	this->output.z = this->inputZ->GetObject<CgDataType::UniformFloat>();
	this->output.w = this->inputW->GetObject<CgDataType::UniformFloat>();
}

