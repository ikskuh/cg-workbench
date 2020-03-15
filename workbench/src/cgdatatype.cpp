#include "cgdatatype.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define X(_Type, _Value) \
    template<> \
    typename UniformType<_Type>::type GetDefaultValue<_Type>() { \
        return _Value; \
    }

X(CgDataType::Audio, AudioStream{})
X(CgDataType::Event, Event{})
X(CgDataType::Shader, ShaderProgram{})
X(CgDataType::Geometry, Geometry{})
X(CgDataType::Texture2D, 0)
X(CgDataType::RenderPass, RenderPass{})
X(CgDataType::UniformFloat, 0.0f)
X(CgDataType::UniformMat3, glm::identity<glm::mat3>())
X(CgDataType::UniformMat4, glm::identity<glm::mat4>())
X(CgDataType::UniformVec2, glm::vec2{0})
X(CgDataType::UniformVec3, glm::vec3{0})
X(CgDataType::UniformVec4, glm::vec4{0})
