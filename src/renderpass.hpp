#pragma once

#include "geometry.hpp"
#include "shaderprogram.hpp"

struct RenderPass
{
    RenderPass();

    Geometry const * geometry;
    ShaderProgram const * shader;
    glm::mat4 transform;
};

