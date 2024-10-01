#pragma once

namespace MSGE
{
class IGLResource
{
public:
    IGLResource();
    virtual ~IGLResource();

    virtual void create() = 0;

     // Bind the resource (e.g., glBindTexture, glUseProgram)
    virtual void bind() = 0;

    // Unbind the resource (optional, depending on resource type)
    virtual void unbind() = 0;

};
}