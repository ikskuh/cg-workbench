#pragma once

#include "window.hpp"
#include "renderpass.hpp"

class RenderPassNode : public Window
{
    WINDOW_PREAMBLE
    WINDOW_CUSTOM_SERIALIZE
private:
    RenderPass pass;
    Sink * geom;
    Sink * shader;
    Sink * transform;
protected:
    void OnRender() override;
    void OnUpdate() override;
public:
    RenderPassNode();
};
