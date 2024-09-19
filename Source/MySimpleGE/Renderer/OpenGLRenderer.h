#pragma once

//Opaque handle to gl context same as SDL handles it
typedef void* GLContext;

namespace MSGE 
{

class OpenGLRenderer {
    OpenGLRenderer() = delete;
    OpenGLRenderer(GLContext context);
    ~OpenGLRenderer();

    void Render();

};
}