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
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "OpenGlFunctions.h"

using namespace glm;


class ModelHolder
{
    public:
        ModelHolder();
        //ModelHolder(std::string name, std::string path);
        virtual ~ModelHolder();

        void drawObject(mat4 mP, mat4 mV, mat4 mM);

        void setObject(float *vert, float* tex, float* norms, int n);

        void IWantToBeaTeapot();


    protected:
        bool loadFromOBJ(std::string path);

    private:
        std::string name;

        ///TODO M matrix for object;
        ///TODO move-me ability;

        unsigned int vertexCount = 0;
        float *vertices;
		float *normals;
		float *vertexNormals;
		float *texCoords;
		float *colors;

		ShaderProgram *shaderProgram;

		GLuint vao;
        GLuint bufVertices; //Uchwyt na bufor VBO przechowuj¹cy tablicê wspó³rzêdnych wierzcho³ków
        GLuint bufColors;  //Uchwyt na bufor VBO przechowuj¹cy tablicê kolorów
        GLuint bufNormals; //Uchwyt na bufor VBO przechowuj¹cy tablickê wektorów normalnych

        std::vector<int> whichVertex;
        std::vector<int> whichTexCord;
        std::vector<int> whichNormal;

        void parseF(std::string line);
        void prepareObject();

        void setGlobalColor(float r, float g, float b, float a);
};

#endif // MODELHOLDER_H
