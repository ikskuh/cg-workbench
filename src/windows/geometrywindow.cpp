#include "geometrywindow.hpp"
#include <stddef.h>
#include <unistd.h>
#include <nfd.h>
#include <tiny_obj_loader.h>

GeometryWindow::GeometryWindow() :
    Window("Geometry"),
    vertices()
{
	glCreateVertexArrays(1, &this->geom.VertexArray);

	glCreateBuffers(1, &this->vertexBuffer);

	glEnableVertexArrayAttrib(this->geom.VertexArray, 0);
	glEnableVertexArrayAttrib(this->geom.VertexArray, 1);
	glEnableVertexArrayAttrib(this->geom.VertexArray, 2);

	glVertexArrayAttribFormat(
		this->geom.VertexArray,
		0, // attrib
		3,
		GL_FLOAT,
		GL_FALSE,
		offsetof(Vertex,position));
	glVertexArrayAttribFormat(
		this->geom.VertexArray,
		1, // attrib
		3,
		GL_FLOAT,
		GL_FALSE,
		offsetof(Vertex,normal));
	glVertexArrayAttribFormat(
		this->geom.VertexArray,
		2, // attrib
		2,
		GL_FLOAT,
		GL_FALSE,
		offsetof(Vertex,uv));

	glVertexArrayVertexBuffer(
		this->geom.VertexArray,
		0,                  // binding index
		this->vertexBuffer,
		0,                  // offset
		sizeof(Vertex)); // stride

	glVertexArrayAttribBinding(
		this->geom.VertexArray,
		0,  // attrib index
		0); // binding index
	glVertexArrayAttribBinding(
		this->geom.VertexArray,
		1,  // attrib index
		0); // binding index
	glVertexArrayAttribBinding(
		this->geom.VertexArray,
		2,  // attrib index
		0); // binding index

	this->LoadQuad();

	this->AddSource(new Source(CgDataType::Geometry, "Shape", &this->geom));
}

GeometryWindow::~GeometryWindow()
{

}

static glm::vec3 tv3(float v[4])
{
	return glm::vec3(v[0],v[1],v[2]);
}

static void ta4(float * d, glm::vec3 v)
{
	d[0] = v[0];
	d[1] = v[1];
	d[2] = v[2];
}

void GeometryWindow::OnUpdate()
{
	char cwd[256];
	if(ImGui::BeginMenuBar())
	{
		if(ImGui::BeginMenu("Geometry"))
		{
			if(ImGui::BeginMenu("Load Primitive"))
			{
				if(ImGui::MenuItem("Quad")) this->LoadQuad();
				ImGui::EndMenu();
			}
			if(ImGui::MenuItem("Load OBJ..."))
			{
				nfdchar_t *outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj", getcwd(cwd, sizeof(cwd)), &outPath );
				if ( result == NFD_OKAY )
				{
					std::string fileName(outPath);
					free(outPath);

					this->LoadObj(fileName);
				}
				else if ( result == NFD_CANCEL )
					; // Silently ignore cancel
				else
				{
					printf("Error: %s\n", NFD_GetError() );
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Text("Triangle Count: %d", this->geom.VertexCount);

	if(ImGui::Button("Fix Normals"))
	{
		for(size_t i = 0; i < this->vertices.size(); i += 3)
		{
			auto & v0 = this->vertices[i+0];
			auto & v1 = this->vertices[i+1];
			auto & v2 = this->vertices[i+2];
			auto normal = glm::normalize(glm::cross(
				glm::normalize(tv3(v1.position) - tv3(v0.position)),
				glm::normalize(tv3(v2.position) - tv3(v0.position))));
			ta4(v0.normal, normal);
			ta4(v1.normal, normal);
			ta4(v2.normal, normal);
		}
		this->Reload();
	}
}

nlohmann::json GeometryWindow::Serialize() const
{
	return {
		{ "type", "geometry" },
		{ "file", this->currentFile }
	};
}


void GeometryWindow::Deserialize(const nlohmann::json &value)
{
	this->currentFile = value.value("file", "*quad");
	if(this->currentFile == "*quad")
		this->LoadQuad();
	else
		this->LoadObj(this->currentFile);
}

void GeometryWindow::Reload()
{
	glNamedBufferData(
		this->vertexBuffer,
		sizeof(Vertex) * this->vertices.size(),
	    vertices.data(),
		GL_STATIC_DRAW);
	this->geom.VertexCount = this->vertices.size();
}

void GeometryWindow::LoadObj(std::string const & fileName)
{
	// Load obj here
	using namespace tinyobj;

	attrib_t attrib;
	std::vector<shape_t> shapes;

	bool success = tinyobj::LoadObj(&attrib, &shapes, nullptr, nullptr, fileName.c_str());
	if(!success)
		return;

	this->currentFile = fileName;

	this->vertices.clear();
	for(shape_t const & sh : shapes)
	{
		for(index_t const & idx : sh.mesh.indices)
		{
			Vertex vtx;

			vtx.position[0] = attrib.vertices[3 * idx.vertex_index + 0];
			vtx.position[1] = attrib.vertices[3 * idx.vertex_index + 1];
			vtx.position[2] = attrib.vertices[3 * idx.vertex_index + 2];

			if(idx.normal_index >= 0)
			{
				vtx.normal[0] = attrib.normals[3 * idx.normal_index + 0];
				vtx.normal[1] = attrib.normals[3 * idx.normal_index + 1];
				vtx.normal[2] = attrib.normals[3 * idx.normal_index + 2];
			}
			else
			{
				memset(vtx.normal, 0, sizeof(float) * 3);
			}

			if(idx.texcoord_index >= 0)
			{
				vtx.uv[0] = attrib.normals[2 * idx.texcoord_index + 0];
				vtx.uv[1] = attrib.normals[2 * idx.texcoord_index + 1];
			}
			else
			{
				memset(vtx.uv, 0, sizeof(float) * 2);
			}

			this->vertices.emplace_back(vtx);
		}
	}
	this->Reload();
}

void GeometryWindow::LoadQuad()
{
#define A 0
#define B 1
	this->currentFile = "*quad";
	this->vertices = {
		Vertex {
			position: { -1.0f, -1.0f, 0.0f },
			normal: { 0,0,1 },
			uv: { 0,A }
		},
		Vertex {
			position: {1.0f, -1.0f, 0.0f},
			normal: {0,0,1},
			uv: {1,A}
		},
		Vertex {
			position: {-1.0f,  1.0f, 0.0f},
			normal: {0,0,1},
			uv: {0,B}
		},
		Vertex {
			position: {1.0f,  1.0f, 0.0f},
			normal: {0,0,1},
			uv: {1,B}
		},
		Vertex {
			position: {1.0f, -1.0f, 0.0f},
			normal: {0,0,1},
			uv: {1,A}
		},
		Vertex {
			position: {-1.0f,  1.0f, 0.0f},
			normal: {0,0,1},
			uv: {0,B}
		},
	};
	this->Reload();
}
