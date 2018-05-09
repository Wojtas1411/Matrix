#include "ModelHolder.h"

ModelHolder::ModelHolder()
{
    //ctor
    std::cout<<"hello there"<<std::endl;
    if(loadFromOBJ("test.obj")){
        std::cout<<"yep"<<std::endl;
    } else {
        std::cout<<"nope"<<std::endl;
    }
    this->vertices = nullptr;
    this->normals = nullptr;
    this->vertexNormals = nullptr;
    this->texCoords = nullptr;
    this->colors = nullptr;
}

ModelHolder::~ModelHolder()
{
    std::cout<<"Destructor started"<<std::endl;
    delete[] this->vertices;
    delete[] normals;
    delete[] vertexNormals;
    delete[] texCoords;
    delete[] colors;


    glDeleteVertexArrays(1,&this->vao); //Usuniêcie vao
	glDeleteBuffers(1,&this->bufVertices); //Usuniêcie VBO z wierzcho³kami
	glDeleteBuffers(1,&this->bufColors); //Usuniêcie VBO z kolorami
	glDeleteBuffers(1,&this->bufNormals); //Usuniêcie VBO z wektorami normalnymi
    std::cout<<"Destructor finished"<<std::endl;
}

void ModelHolder::parseF(std::string line){
    std::vector<std::string> str_vector;

    unsigned long iter = 0;
    unsigned long prev = 0;
    while(iter<line.size()){
        if(line.find(' ',prev+1)>0) iter = line.find(' ',prev+1);
        else iter = line.size();
        str_vector.push_back(line.substr(prev,iter-prev+1));
        prev = iter+1;
    }

    for(int i = 0;i<(int)str_vector.size();i++){
        if(str_vector[i].empty())continue;
        if(str_vector[i].find(' ',0)!=std::string::npos){
                str_vector[i].erase(str_vector[i].size()-1,1);
        }
        if(str_vector[i].find(' ',0)!=std::string::npos){
                std::cout<<"parsing error spaces found"<<std::endl;;
                //str_vector[i].erase(str_vector[i].size()-1,1);
        }
        //std::cout<<str_vector[i]<<std::endl;
    }

    int tmp0, tmp1, tmp2;

    for(int i=0;i<(int)str_vector.size();i++){
        if(str_vector[i].empty())continue;
        if(str_vector[i].find('/')==str_vector[i].rfind('/')){
            ///vetices + textures
            tmp0 = strtol(str_vector[i].substr(0,str_vector[i].find('/')).c_str(),nullptr,0);
            tmp1 = strtol(str_vector[i].substr(str_vector[i].find('/')+1,str_vector[i].size()-str_vector[i].find('/')).c_str(),nullptr,0);
            tmp2 = -1;
            //std::cout<<tmp0<<" "<<tmp1<<std::endl;
        } else if(str_vector[i].find('/')+1==str_vector[i].rfind('/')){
            ///vertices + normals
            tmp0 = strtol(str_vector[i].substr(0,str_vector[i].find('/')).c_str(),nullptr,0);
            tmp1 = -1;
            tmp2 = strtol(str_vector[i].substr(str_vector[i].find('/')+2,str_vector[i].size()-str_vector[i].find('/')).c_str(),nullptr,0);
        } else if(str_vector[i].find('/')==std::string::npos){
            ///vertices only
            tmp0 = strtol(str_vector[i].c_str(),nullptr,0);
            tmp1 = -1;
            tmp2 = -1;
        } else {
            ///the most important faces loader - all of them
            tmp0 = strtol(str_vector[i].substr(0,str_vector[i].find('/')).c_str(),nullptr,0);
            tmp1 = strtol(str_vector[i].substr(str_vector[i].find('/')+1,str_vector[i].rfind('/')-str_vector[i].find('/')).c_str(),nullptr,0);
            tmp2 = strtol(str_vector[i].substr(str_vector[i].rfind('/')+1,str_vector[i].size()-str_vector[i].rfind('/')).c_str(),nullptr,0);
            //std::cout<<tmp0<<" "<<tmp1<<" "<<tmp2<<std::endl;
        }

        if(tmp0!=-1){
            whichVertex.push_back(tmp0-1);
        }
        if(tmp1!=-1){
            whichTexCord.push_back(tmp1-1);
        }
        if(tmp2!=-1){
            whichNormal.push_back(tmp2-1);
        }
    }

}


bool ModelHolder::loadFromOBJ(std::string path){
    std::ifstream in;
    in.open(path.c_str());

    if(!in.is_open()){
        std::cout<<"[load OBJ] file error: "<<this->name<<std::endl;
        return false;
    }


    std::string tmp;
    float tmp_num;

    std::vector<float> tmp_vertices;
    std::vector<float> tmp_normals;
    std::vector<float> tmp_vertexNormals;
    std::vector<float> tmp_texCoords;
    std::vector<float> tmp_colors;

    while(!in.eof()){
        in>>tmp;
        if(tmp == "v"){
            for(int i=0;i<3;i++){
                in>>tmp_num;
                tmp_vertices.push_back(tmp_num);
            }
            //std::cout<<"v"<<std::endl;
        }else if(tmp == "vt"){
            for(int i=0;i<2;i++){
                in>>tmp_num;
                tmp_texCoords.push_back(tmp_num);
            }
            //std::cout<<"vt"<<std::endl;
        }else if(tmp == "vn"){
            for(int i=0;i<3;i++){
                in>>tmp_num;
                tmp_normals.push_back(tmp_num);
            }
            //std::cout<<"vn"<<std::endl;
        }else if(tmp == "f"){
            getline(in,tmp);
            ///TODO f parser
            this->parseF(tmp.erase(0,1));
            //std::cout<<"face "<<tmp<<std::endl;
        }else if(tmp == "#"){
            getline(in,tmp);
            //std::cout<<"[loadOBJ] comment: "<<tmp<<std::endl;
        }else{
            getline(in,tmp);
            //std::cout<<"[loadOBJ] cannot parse the line: "<<tmp<<std::endl;
        }
    }
    in.close();
    std::cout<<"Reading finished"<<std::endl;
    ///TODO process data
    std::cout<<whichVertex.size()<<std::endl;
    std::cout<<whichTexCord.size()<<std::endl;
    std::cout<<whichNormal.size()<<std::endl;

    vertices = new float[whichVertex.size()*4];
    texCoords = new float[whichTexCord.size()*2];
    normals = new float[whichNormal.size()*4];

    for(int i=0;i<whichVertex.size();i++)std::cout<<whichVertex[i]<<std::endl;std::cout<<std::endl;

    vertexCount = whichVertex.size();

    for(int i=0; i<vertexCount; i++){
        std::cout<<whichVertex[i]<<std::endl;
        for(int j=0;j<3;j++){
            vertices[4*i+j] = tmp_vertices[whichVertex[i]*3+j];
            normals[4*i+j] = tmp_normals[whichNormal[i]*3+j];
        }
        vertices[4*i+3] = 1.0f;
        normals[4*i+3] = 0.0f;
        for(int j=0;j<2;j++){
            texCoords[2*i+j] = tmp_texCoords[whichTexCord[i]*2+j];
        }
    }

    for(int i=0;i<vertexCount;i++){
        std::cout<<vertices[4*i]<<" "<<vertices[4*i+1]<<" "<<vertices[4*i+2]<<" "<<vertices[4*i+3]<<std::endl;
    }

    std::cout<<"Parsing finished"<<std::endl;

    tmp_colors.clear();
    tmp_normals.clear();
    tmp_texCoords.clear();
    tmp_vertexNormals.clear();
    tmp_vertices.clear();
    whichVertex.clear();
    whichTexCord.clear();
    whichNormal.clear();

    return true;
}

void ModelHolder::drawMe(){


}
