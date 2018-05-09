#ifndef MODELHOLDER_H
#define MODELHOLDER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

using namespace glm;


class ModelHolder
{
    public:
        ModelHolder();
        //ModelHolder(std::string name, std::string path);
        virtual ~ModelHolder();

        void drawMe();

    protected:
        bool loadFromOBJ(std::string path);

    private:
        std::string name;

        int vertexCount = 0;
        float *vertices;
		float *normals;
		float *vertexNormals;
		float *texCoords;
		float *colors;

		GLuint vao;
        GLuint bufVertices; //Uchwyt na bufor VBO przechowuj�cy tablic� wsp�rz�dnych wierzcho�k�w
        GLuint bufColors;  //Uchwyt na bufor VBO przechowuj�cy tablic� kolor�w
        GLuint bufNormals; //Uchwyt na bufor VBO przechowuj�cy tablick� wektor�w normalnych

        std::vector<int> whichVertex;
        std::vector<int> whichTexCord;
        std::vector<int> whichNormal;

        void parseF(std::string line);
};

#endif // MODELHOLDER_H
