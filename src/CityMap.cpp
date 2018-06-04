#include "CityMap.h"

CityMap::CityMap(int mX, int mY, ModelHolder **tab_of_blocks, ModelHolder *dach, ModelHolder **roads)
{
    this->mX = mX;
    this->mY = mY;
    //ctor
    generate_map(tab_of_blocks,dach,roads);
    generate_map_hei();
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
    for(int i=0;i<this->mapa_hei_size;i++){
        delete[] mapa_hei[i];
    }
    delete[] mapa_hei;
    //dtor
}
///values in map
///*** -1 -> this is default value, if it appears in final form, that means mistake
///*** 0-x(15?) -> types of buildings, not defined yet
///*** -2 -> this means road

void CityMap::generate_map(ModelHolder **x, ModelHolder *dach, ModelHolder **roads){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0,4);

    if(this->mapa == nullptr){
        mapa = new int*[this->map_size];
        for(int i=0;i<this->map_size;i++){
            mapa[i] = new int[this->map_size];
            for(int j=0;j<this->map_size;j++){
                mapa[i][j] = -1;

                if(i%3==0 or j%3==0){mapa[i][j] = -2; } ///road

                else { mapa[i][j] = distribution(generator);} ///budynek
            }
        }
    }
    if(this->buildings == nullptr){
        buildings = new Building**[this->map_size];
        for(int i=0;i<this->map_size;i++){
            buildings[i] = new Building*[this->map_size];
            for(int j=0;j<this->map_size;j++){
                buildings[i][j] = nullptr;

                if(i%3!=0 and j%3!=0){ buildings[i][j] = new Building(i,j,mapa[i][j],x,dach);}///budynki

                else if(i%3!=2 and j%3!= 2){
                    if(buildings[i][j] == nullptr){buildings[i][j] = new RoadHolder(i, j, roads);}}///drogi
            }
        }
    }
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

            if(i%3!=0 and j%3!=0){if(buildings != nullptr)buildings[i][j]->drawBuilding(P,V,light_position);}///budynki

            else if(i%3!=2 and j%3!= 2){if(buildings != nullptr)buildings[i][j]->drawBuilding(P,V,light_position);}///drogi

            //std::cout<<i<<" "<<j<<" xDDDDDDDD"<<std::endl;
        }
    }
}

void CityMap::generate_map_hei(){
    mapa_hei_size = this->map_size*2*4;
    mapa_hei = new float*[mapa_hei_size];
    for(int i=0;i<mapa_hei_size;i++){
        mapa_hei[i] = new float[mapa_hei_size];
        for(int j=0;j<mapa_hei_size;j++){
            switch(mapa[i/8][j/8]){
            case -2:
                mapa_hei[i][j] = 0;
                break;
            case -1:
                std::cout<<"ERROR"<<std::endl;
            default:
                mapa_hei[i][j] = buildings[i/8][j/8]->get_number_of_segments()*Building::segment_heights[mapa[i/8][j/8]]*4;
            }
        }
    }

}

void CityMap::print_map_hei(int x, int y){
    if(mapa_hei!= nullptr){
        std::cout<<mapa_hei[x][y]<<std::endl;
    }
    else{
        std::cout<<"error"<<std::endl;
    }
}

float** CityMap::get_map_height(){
    return this->mapa_hei;
}
