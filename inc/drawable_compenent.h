#ifndef DRAWABLE_COMPONENT_H_INCLUDED
#define DRAWABLE_COMPONENT_H_INCLUDED
#include <common.h>
class drawable_component
{
public:
    virtual void draw(NDC pos) = 0;
};

#endif