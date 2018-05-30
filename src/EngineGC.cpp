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
    if(h+0.5f < map_hei[x][y]){
            std::cout<<"collision simple "<<map_hei[x][y]<<std::endl;
            v_falling = 0;
            return position_old;
    }
    return position;
}

glm::vec3 EngineGC::collisions_advanced(glm::vec3 position, glm::vec3 position_old){
    int x = (int)position.x+4;
    int y = (int)position.z+4;
    float h = position.y - 3.0f;
    if(h+0.3f < map_hei[x][y]){
            std::cout<<"collision advanced "<<map_hei[x][y]<<std::endl;
            v_falling = 0;
            return position_old;
    }
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i==0 and j==0){
                ///do nothing
            }else if(h+0.3f<map_hei[x+i][y+j]){
                v_falling = 0;
                return position_old;
            }
        }
    }
    return position;
}

glm::vec3 EngineGC::gravity_falling(glm::vec3 position){
    int x = (int)position.x+4;
    int y = (int)position.z+4;
    float h = position.y - 3.0f;

    if(h<=map_hei[x][y]+0.3f){
        in_air = false;
        v_falling = 0;
        h = map_hei[x][y];
    }
    else{
        in_air = true;
        v_falling += g_gravity*time_period;
        h -= v_falling*time_period;
    }

    position.y = h+3.0f;


    return position;
}

glm::vec3 EngineGC::gravity_advanced(glm::vec3 position, bool jump){
    int x = (int)position.x+4;
    int y = (int)position.z+4;
    float h = position.y - 3.0f;

    if(jump and !in_air){
            std::cout<<"jump triggered"<<std::endl;
        v_falling -= v_max_jump;
        in_air = true;
        //v_falling += g_gravity*time_period;
        h -= v_falling*time_period;
    }

    if(h<=map_hei[x][y]+0.3f){
        in_air = false;
        v_falling = 0;
        h = map_hei[x][y];
    }
    else{
        in_air = true;
        v_falling += g_gravity*time_period;
        h -= v_falling*time_period;
    }

    position.y = h+3.0f;


    return position;
}

bool EngineGC::check_advanced_collisions_avalible(glm::vec3 position){
    int x = (int)position.x+4;
    int y = (int)position.z+4;
    float h = position.y - 3.0f;
    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if(i==0 and j==0){
                ///do nothing
            }else if(h+0.3f<map_hei[x+i][y+j]){
                return false;
            }
        }
    }
    return true;
}
