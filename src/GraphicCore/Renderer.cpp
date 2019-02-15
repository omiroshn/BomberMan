#include "GraphicCore/Renderer.hpp"

Renderer::Renderer()
{
};

Renderer::~Renderer()
{
};

void Renderer::draw(/*std::vector<Instance> const& whatToDraw*/)
{
    normalPass(/*whatToDraw*/);
}

void Renderer::normalPass(/*std::vector<Instance> const& instances*/)
{
    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}