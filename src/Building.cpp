#include "Building.h"
Building::Building(){}
/*
Building::Building()
{
    //ctor
    std::cout<<"ERROR: default building constructor"<<std::endl;
    this->mX = 1;
    this->mY = 1;
    this->my_type = 1;
    //this->box = boxes;
    //this->dach = dach;
}
*/
Building::Building(int mX, int mY,int type, ModelHolder **boxes, ModelHolder *dach){
    this->mX = mX;
    this->mY = mY;
    this->my_type = type;
    this->box = boxes;
    this->dach = dach;

    this->generate_number_of_segments();
    this->generate_real_coords();
    this->generate_M_ground();
    this->generate_M_vector();
}

Building::~Building()
{
    M_vector.clear();
    //dtor
}

void Building::generate_number_of_segments(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    //std::default_random_engine generator (time(0));
    //std::uniform_real_distribution<float>distribution(40.0f,88.0f);
    std::normal_distribution<float> distribution(64.0,16.0);
    float height = distribution(generator);
    this->number_of_segments = height/(this->segment_heights[this->my_type]*this->global_scalar);
    //std::cout<<height<<" "<<number_of_segments<<std::endl;
}

void Building::generate_real_coords(){
    pX = (float)(mX)*2.0f*this->global_scalar;
    pY = (float)(mY)*2.0f*this->global_scalar;
}

void Building::generate_M_ground(){
    glm::mat4 M = glm::mat4(1.0f);
    //M = scale(M,glm::vec3(global_scalar,global_scalar,global_scalar));
    M = translate(M,glm::vec3(pX,0,pY));
    M = scale(M,glm::vec3(global_scalar,global_scalar,global_scalar));

    this->M_vector.push_back(M);
}

void Building::generate_M_vector(){
    for(int i=1;i<number_of_segments+1;i++){
        this->M_vector.push_back(translate(M_vector[i-1],glm::vec3(0,segment_heights[my_type],0)));
    }
}

void Building::drawBuilding(glm::mat4 P, glm::mat4 V, glm::vec4 pos){
    //std::cout<<my_type<<std::endl;
    for(int i=0;i<number_of_segments;i++){
        //this->box->drawObject(P,V,M_vector[i],pos);
        this->box[my_type]->drawObject(P,V,M_vector[i],pos);
    }
    this->dach->drawObject(P,V,M_vector[number_of_segments],pos);
}

void Building::print_data(){
    std::cout<<mX<<" "<<mY<<std::endl;
    std::cout<<pX<<" "<<pY<<std::endl;
    std::cout<<my_type<<" "<<number_of_segments<<std::endl;
}

int Building::get_number_of_segments(){
    return number_of_segments;
}
