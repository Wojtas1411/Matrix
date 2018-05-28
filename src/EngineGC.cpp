#include "EngineGC.h"

EngineGC::EngineGC(float** map_hei)
{
    this->map_hei = map_hei;
    //ctor
}

EngineGC::~EngineGC()
{
    //dtor
}

glm::vec3 EngineGC::collisions_simple(glm::vec3 position, glm::vec3 position_old){
    int x = (int)position.x+4;
    int y = (int)position.z+4;
    float h = position.y - 3.0f;
    if(h < map_hei[x][y]){
            std::cout<<"collision"<<std::endl;
            return position_old;
    }
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i==0 and j==0){
                ///do nothing
            }else if(h<map_hei[x+i][y+j]){
                return position_old;
            }
        }
    }
    return position;
}
