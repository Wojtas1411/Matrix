#include "Box.h"

Box::Box(int i)
{
    if(i>LAST)i = LAST;
    if(i<FIRST)i = FIRST;
    std::string num = " ";
    num[0] = char(48+i%10);
    if(i>=10)num = char(48+i/10) + num;
    std::string temp1 = "objects/box" + num + ".obj";
    std::string temp2 = "textures/tex" + num + ".png";
    std::string temp3 = temp2;
    this->loadModel(temp1.c_str(),temp2.c_str(),temp3.c_str());
    //ctor
}

Box::~Box()
{
    //dtor
}
