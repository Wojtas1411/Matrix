#include "dach.h"

//dach::dach() : ModelHolder("dach.obj","textures/beton.png","textures/beton.png")
//dach::dach() : ModelHolder(object_name, texture0_name, texture1_name)
dach::dach()
{
    this->loadModel("objects/dach.obj","textures/beton.png","textures/beton.png");
}

dach::~dach()
{
    //dtor
}
