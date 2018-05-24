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

    protected:

    private:
        int mX, mY;


        int **mapa = nullptr;

        Building ***buildings = nullptr;

        ///TODO roads;

        int map_size = 1002;

        const int RANGE_SMALL = 10;
        const int RANGE_MEDIUM = 20;
        const int RANGE_LARGE = 30;

        int current_range = RANGE_SMALL;

        void generate_map(ModelHolder **x, ModelHolder *dach, ModelHolder **roads);

};

#endif // CITYMAP_H
