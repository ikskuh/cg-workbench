#include "utils.hpp"

namespace utils
{
    char const * GetTextureFormatName(GLenum format)
    {
        switch(format)
        {
#define X(_Format) case _Format: return #_Format;
            X(GL_DEPTH_COMPONENT)
            X(GL_DEPTH_STENCIL)
            X(GL_RED)
            X(GL_RG)
            X(GL_RGB)
            X(GL_RGBA)
            X(GL_R8)
            X(GL_R8_SNORM)
            X(GL_R16)
            X(GL_R16_SNORM)
            X(GL_RG8)
            X(GL_RG8_SNORM)
            X(GL_RG16)
            X(GL_RG16_SNORM)
            X(GL_R3_G3_B2)
            X(GL_RGB4)
            X(GL_RGB5)
            X(GL_RGB8)
            X(GL_RGB8_SNORM)
            X(GL_RGB10)
            X(GL_RGB12)
            X(GL_RGB16_SNORM)
            X(GL_RGBA2)
            X(GL_RGBA4)
            X(GL_RGB5_A1)
            X(GL_RGBA8)
            X(GL_RGBA8_SNORM)
            X(GL_RGB10_A2)
            X(GL_RGB10_A2UI)
            X(GL_RGBA12)
            X(GL_RGBA16)
            X(GL_SRGB8)
            X(GL_SRGB8_ALPHA8)
            X(GL_R16F)
            X(GL_RG16F)
            X(GL_RGB16F)
            X(GL_RGBA16F)
            X(GL_R32F)
            X(GL_RG32F)
            X(GL_RGB32F)
            X(GL_RGBA32F)
            X(GL_R11F_G11F_B10F)
            X(GL_RGB9_E5)
            X(GL_R8I)
            X(GL_R8UI)
            X(GL_R16I)
            X(GL_R16UI)
            X(GL_R32I)
            X(GL_R32UI)
            X(GL_RG8I)
            X(GL_RG8UI)
            X(GL_RG16I)
            X(GL_RG16UI)
            X(GL_RG32I)
            X(GL_RG32UI)
            X(GL_RGB8I)
            X(GL_RGB8UI)
            X(GL_RGB16I)
            X(GL_RGB16UI)
            X(GL_RGB32I)
            X(GL_RGB32UI)
            X(GL_RGBA8I)
            X(GL_RGBA8UI)
            X(GL_RGBA16I)
            X(GL_RGBA16UI)
            X(GL_RGBA32I)
            X(GL_RGBA32UI)
            X(GL_COMPRESSED_RED)
            X(GL_COMPRESSED_RG)
            X(GL_COMPRESSED_RGB)
            X(GL_COMPRESSED_RGBA)
            X(GL_COMPRESSED_SRGB)
            X(GL_COMPRESSED_SRGB_ALPHA)
            X(GL_COMPRESSED_RED_RGTC1)
            X(GL_COMPRESSED_SIGNED_RED_RGTC1)
            X(GL_COMPRESSED_RG_RGTC2)
            X(GL_COMPRESSED_SIGNED_RG_RGTC2)
            X(GL_COMPRESSED_RGBA_BPTC_UNORM)
            X(GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM)
            X(GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT)
            X(GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT)
#undef X
            default: return "???";
        }
    }







}