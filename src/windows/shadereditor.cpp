#include "shadereditor.hpp"
#include <cstdio>

char const * defaultVertexShader = R"glsl(#version 330
layout(location = 0) in vec3 position;

out vec2 uv;

void main() {
	uv = 0.5 + 0.5 * position.xy;
	gl_Position = vec4(position, 1.0);
}
)glsl";

char const * defaultFragmentShader = R"glsl(#version 330
out vec4 color;

in vec2 uv;

void main() {
	color = vec4(uv,0,1);
}
)glsl";

static const char * shaderToString(GLenum shaderType)
{
	switch(shaderType)
	{
		case GL_VERTEX_SHADER:          return "Vertex Shader";
		case GL_TESS_CONTROL_SHADER:    return "Tess. Control Shader";
		case GL_TESS_EVALUATION_SHADER: return "Tess. Eval. Shader";
		case GL_GEOMETRY_SHADER:        return "Geometry Shader";
		case GL_FRAGMENT_SHADER:        return "Fragment Shader";
		case GL_COMPUTE_SHADER:         return "Computer Shader";
		default:                        return "Invalid";
	}
}

Shader::Shader(GLenum shaderType) :
    id(glCreateShader(shaderType)),
    type(shaderType),
    compiledSuccessfully(false),
    infoLog("Compile the shader for the first time!")
{
	switch(shaderType)
	{
		case GL_VERTEX_SHADER:
			strcpy(this->code, defaultVertexShader);
			break;
		case GL_FRAGMENT_SHADER:
			strcpy(this->code, defaultFragmentShader);
			break;
		default:
			strcpy(this->code, "// Write your shader here!");
			break;
	}
}

Shader::~Shader()
{
	glDeleteShader(this->id);
}

void Shader::Compile()
{
	char const * src = this->code;
	glShaderSource(this->id, 1, &src, nullptr);

	glCompileShader(this->id);

	GLint status;
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &status);
	this->compiledSuccessfully = (status == GL_TRUE);
	if(status == GL_FALSE)
	{
		int len;
		glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> log(len + 1);
		glGetShaderInfoLog(
			this->id,
			log.size(),
			nullptr,
			log.data());
		this->infoLog = std::string(log.data());
	}
	else
	{
		this->infoLog = "";
	}
}

ShaderEditor::ShaderEditor() :
    Window("Shader Editor"),
	program(glCreateProgram()),
    shaders(),
    shaderLog()
{
	this->AddSource(new Source(CgDataType::Shader, "Shader", &this->program));

	this->shaders.emplace_back(new Shader(GL_VERTEX_SHADER));
	this->shaders.emplace_back(new Shader(GL_FRAGMENT_SHADER));

	this->Compile();
}

ShaderEditor::~ShaderEditor()
{
	glDeleteProgram(this->program);
}

void ShaderEditor::OnRender()
{
	int index = 0;
	for(Uniform const & u : this->uniforms)
	{
		switch(u.sink->GetType())
		{
			case CgDataType::UniformFloat:
			{
				glProgramUniform1f(this->program, u.location, u.sink->GetObject<CgDataType::UniformFloat>());
				break;
			}
			case CgDataType::UniformVec2:
			{
				auto val = u.sink->GetObject<CgDataType::UniformVec2>();
				glProgramUniform2f(this->program, u.location, val.x, val.y);
				break;
			}
			case CgDataType::UniformVec3:
			{
				auto val = u.sink->GetObject<CgDataType::UniformVec3>();
				glProgramUniform3f(this->program, u.location, val.x, val.y, val.z);
				break;
			}
			case CgDataType::UniformVec4:
			{
				auto val = u.sink->GetObject<CgDataType::UniformVec4>();
				glProgramUniform4f(this->program, u.location, val.x, val.y, val.z, val.w);
				break;
			}
			case CgDataType::UniformMat3:
			{
				auto val = u.sink->GetObject<CgDataType::UniformMat3>();
				glProgramUniformMatrix3fv(this->program, u.location, 1, GL_FALSE, &val[0].x);
				break;
			}
			case CgDataType::UniformMat4:
			{
				auto val = u.sink->GetObject<CgDataType::UniformMat4>();
				glProgramUniformMatrix4fv(this->program, u.location, 1, GL_FALSE, &val[0].x);
				break;
			}
			case CgDataType::Texture2D:
			{
				GLuint img = u.sink->GetObject<CgDataType::Texture2D>();

				glProgramUniform1i(this->program, u.location, index);

				glBindTextureUnit(index, img);

				index++;

				break;
			}
			default:
				fprintf(stderr, "Unsupported type for uniform %s: %d\n", u.name.c_str(), static_cast<int>(u.sink->GetType()));
				break;
		}
	}
}

void ShaderEditor::OnUpdate()
{
	char buffer[64];
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Compile"))
		{
			this->Compile();
			ImGui::CloseCurrentPopup(); // HACK: When menu is opened, menu is closed
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Add"))
		{
			if(ImGui::MenuItem("Vertex Shader"))
				this->shaders.emplace_back(new Shader(GL_VERTEX_SHADER));

			if(ImGui::MenuItem("Tess. Control Shader"))
				this->shaders.emplace_back(new Shader(GL_TESS_CONTROL_SHADER));

			if(ImGui::MenuItem("Tess. Eval. Shader"))
				this->shaders.emplace_back(new Shader(GL_TESS_EVALUATION_SHADER));

			if(ImGui::MenuItem("Geometry Shader"))
				this->shaders.emplace_back(new Shader(GL_GEOMETRY_SHADER));

			if(ImGui::MenuItem("Fragment Shader"))
				this->shaders.emplace_back(new Shader(GL_FRAGMENT_SHADER));

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if(this->shaderLog.length() > 0)
	{
		ImGui::TextWrapped("%s", this->shaderLog.c_str());
		if(ImGui::Button("OK")) this->shaderLog = "";
	}

	for(auto const & sh : this->shaders)
	{
		std::sprintf(buffer, "%s##%p", shaderToString(sh->GetType()), sh.get());
		if(!sh->IsCompiled())
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0,0,0,1.0));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0,0.25,0.25,1.0));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0,0.33,0.33,1.0));
		}
		if(ImGui::CollapsingHeader(buffer))
		{
			std::sprintf(buffer, "##Code Editor %p", sh.get());
			ImGui::InputTextMultiline(
				buffer,
				sh->GetCode(), Shader::MaxLength,
				ImVec2(-1.0f, -1.0),
				ImGuiInputTextFlags_AllowTabInput);

			ImGui::TextWrapped("%s", sh->GetLog().c_str());
		}
		if(!sh->IsCompiled())
			ImGui::PopStyleColor(3);
	}
}

void ShaderEditor::Compile()
{
	bool success = true;
	for(auto const & sh : this->shaders)
	{
		sh->Compile();
		success &= sh->IsCompiled();
	}

	if(success)
	{
		for(auto const & sh : this->shaders)
			glAttachShader(this->program, sh->GetObject());

		glLinkProgram(this->program);

		GLint status;
		glGetProgramiv(this->program, GL_LINK_STATUS, &status);
		if(status == GL_FALSE)
		{
			int len;
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &len);
			std::vector<char> log(len + 1);
			glGetProgramInfoLog(
				this->program,
				log.size(),
				nullptr,
				log.data());
			this->shaderLog = std::string(log.data());
		}
		else
		{
			this->shaderLog = "";

			// remove all sinks and store the connections
			std::map<std::string, Source const *> previous;
			while(this->GetSinkCount() > 0)
			{
				Sink * sink = this->GetSink(0);
				if(sink->HasSourceConnected())
					previous.emplace(sink->GetName(), sink->GetSource(false));
				this->RemoveSink(sink);
			}

			this->uniforms.clear();

			int count;
			glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &count);
			for(int i = 0; i < count; i++)
			{
				GLchar name[256];
				GLint size;
				GLenum type;
				GLsizei len;

				glGetActiveUniform(
					this->program,
					i,
					256,
					&len,
					&size,
					&type,
					name);

				Sink * sink = nullptr;
				switch(type)
				{
					case GL_FLOAT:      sink = new Sink(CgDataType::UniformFloat, std::string(name, len)); break;
					case GL_FLOAT_VEC2: sink = new Sink(CgDataType::UniformVec2,  std::string(name, len)); break;
					case GL_FLOAT_VEC3: sink = new Sink(CgDataType::UniformVec3,  std::string(name, len)); break;
					case GL_FLOAT_VEC4: sink = new Sink(CgDataType::UniformVec4,  std::string(name, len)); break;
					case GL_FLOAT_MAT3: sink = new Sink(CgDataType::UniformMat3,  std::string(name, len)); break;
					case GL_FLOAT_MAT4: sink = new Sink(CgDataType::UniformMat4,  std::string(name, len)); break;
					case GL_SAMPLER_2D: sink = new Sink(CgDataType::Texture2D,    std::string(name, len)); break;
					default:            sink = nullptr; break;
				}

				if(sink != nullptr)
				{
					auto it = previous.find(sink->GetName());
					if(it != previous.end())
					{
						if((it->second != nullptr) && (it->second->GetType() == sink->GetType()))
							sink->SetSource(it->second);
					}

					Uniform u;
					u.sink = sink;
					u.location = i;
					u.name = sink->GetName();

					uniforms.emplace_back(u);

					this->AddSink(sink);
				}
			}
		}

		for(auto const & sh : this->shaders)
			glDetachShader(this->program, sh->GetObject());
	}
}

nlohmann::json ShaderEditor::Serialize() const
{
	using namespace nlohmann;

	json result;
	result["type"] = "shader";

	json shaders;
	for(auto const & sh : this->shaders)
	{
		shaders += {
			{ "type", sh->GetType() },
			{ "code", sh->GetCode() },
		};
	}

	result["items"] = shaders;

	return result;
}

void ShaderEditor::Deserialize(const nlohmann::json &value)
{
	this->shaders.clear();
	for(auto const & shader : value["items"])
	{
		auto * sh = new Shader(shader["type"].get<int>());
		strcpy(sh->GetCode(), shader["code"].get<std::string>().c_str());
		this->shaders.emplace_back(sh);
	}
	this->Compile();
}
