#include "CityMap.h"

CityMap::CityMap(int mX, int mY, ModelHolder **tab_of_blocks, ModelHolder *dach, ModelHolder **roads)
{
    this->mX = mX;
    this->mY = mY;
    //ctor
    generate_map(tab_of_blocks,dach,roads);
}

CityMap::~CityMap()
{
    for(int i=0;i<this->map_size;i++){
        delete[] mapa[i];
        for(int j=0;j<<this->map_size;j++){
            delete buildings[i][j];
        }
        delete[] buildings[i];
    }
    delete[] mapa;
    delete[] buildings;
    //dtor
}
///values in map
///*** -1 -> this is default value, if it appears in final form, that means mistake
///*** 0-x(15?) -> types of buildings, not defined yet
///*** -2 -> this means road

void CityMap::generate_map(ModelHolder **x, ModelHolder *dach, ModelHolder **roads){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0,10);
    if(this->mapa == nullptr){
        mapa = new int*[this->map_size];
        for(int i=0;i<this->map_size;i++){
            mapa[i] = new int[this->map_size];
            for(int j=0;j<this->map_size;j++){
                mapa[i][j] = -1;
                if(i%3==0 or j%3==0){
                    mapa[i][j] = -2; ///road
                }
                else{
                    //mapa[i][j] = distribution(generator);
                    mapa[i][j] = 1;///temporary for testing
                }
            }
        }
    }
    if(this->buildings == nullptr){
        buildings = new Building**[this->map_size];
        for(int i=0;i<this->map_size;i++){
            buildings[i] = new Building*[this->map_size];
            for(int j=0;j<this->map_size;j++){
                buildings[i][j] = nullptr;
                if(i%3!=0 and j%3!=0){
                    //std::cout<<i<<" "<<j<<" "<<mapa[i][j]<<std::endl;
                    buildings[i][j] = new Building(i,j,mapa[i][j],x,dach);
                }
                else if(i%3!=2 and j%3!= 2){
                    if(buildings[i][j] == nullptr){
                            //std::cout<<"make road"<<std::endl;
                        buildings[i][j] = new RoadHolder(i, j, roads);
                    }
                }
            }
        }
    }
    ///TODO roads
}

void CityMap::setPosition(int mX, int mY){
    this->mX = mX;
    this->mY = mY;
}

void CityMap::drawCityMap(glm::mat4 P, glm::mat4 V, glm::vec4 light_position){
    for(int i = max(mX-current_range,0);i<min(mX+current_range,map_size);i++){
        for(int j = max(mY-current_range,0);j<min(mY+current_range,map_size);j++){
            //std::cout<<"xDDD"<<std::endl;
            //if(i%3!=0 and j%3!=0)if(buildings != nullptr)buildings[i][j]->print_data();else std::cout<<"what???"<<std::endl;
            if(i%3!=0 and j%3!=0){if(buildings != nullptr)buildings[i][j]->drawBuilding(P,V,light_position);}
            else if(i%3!=2 and j%3!= 2){if(buildings != nullptr)buildings[i][j]->drawBuilding(P,V,light_position);}
            ///TODO draw roads
            //std::cout<<i<<" "<<j<<" xDDDDDDDD"<<std::endl;
        }
    }
}
