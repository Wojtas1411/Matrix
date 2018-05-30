#ifndef CITYMAP_H
#define CITYMAP_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <chrono>
#include "ModelHolder.h"
#include "dach.h"
#include "Building.h"
#include "RoadHolder.h"

class CityMap
{
    public:
        CityMap();
        CityMap(int mX, int mY, ModelHolder **tab_of_blocks, ModelHolder *dach, ModelHolder **roads);
        virtual ~CityMap();

        void setPosition(int mX, int mY);
        void drawCityMap(glm::mat4 P, glm::mat4 V, glm::vec4 light_position);

        void print_map_hei(int x, int y);

        float** get_map_height();

    protected:

    private:
        int mX, mY;


        int **mapa = nullptr;

        float **mapa_hei = nullptr;

        Building ***buildings = nullptr;

        ///TODO roads;

        int map_size = 1002;

        const int RANGE_SMALL = 10;
        const int RANGE_MEDIUM = 15;
        const int RANGE_LARGE = 20;

        int current_range = RANGE_MEDIUM;

        void generate_map(ModelHolder **x, ModelHolder *dach, ModelHolder **roads);

        void generate_map_hei();
        int mapa_hei_size;

};

#endif // CITYMAP_H
