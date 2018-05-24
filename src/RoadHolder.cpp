#include "RoadHolder.h"

RoadHolder::RoadHolder(int mX, int mY, ModelHolder **boxes)
{
    this->mX = mX;
    this->mY = mY;
    this->box = boxes;
    this->dach = nullptr;
    if(mX%3 == 0 and mY%3 == 0)this->my_type = 0;
    else this->my_type = 1;
    //ctor
    this->generate_real_coords();
    this->generate_M_ground();
}

RoadHolder::~RoadHolder()
{
    //dtor
}

void RoadHolder::generate_real_coords(){
    pX = (float)(mX)*2.0f*this->global_scalar;
    pY = (float)(mY)*2.0f*this->global_scalar;
    if(mX%3 == 0 and mY%3 == 1) pY += global_scalar;
    if(mX%3 == 1 and mY%3 == 0) pX += global_scalar;
}

void RoadHolder::generate_M_ground(){

    this->M = glm::mat4(1.0f);
    M = glm::translate(M,glm::vec3(pX,0,pY));
    M = glm::scale(M,glm::vec3(global_scalar,global_scalar,global_scalar));
    if(mX%3 == 0 and mY%3 == 1)M = glm::rotate(M,(float)glm::radians(90.0),glm::vec3(0,1,0));
}

void RoadHolder::drawBuilding(glm::mat4 P, glm::mat4 V, glm::vec4 pos){
    //std::cout<<2<<std::endl;
    this->box[my_type]->drawObject(P,V,M,pos);
}
