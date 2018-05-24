#ifndef BUILDING_H
#define BUILDING_H

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
/***we use this class to draw a building from boxes
what we do here:
get random height of the building
generate M matrices for each Box and roof (dach)
we use preloaded data
>height of segment
in constructor we set x and y coordinates for the building (on the city map)

***/

class Building
{
    public:
        Building();
        Building(int mX, int mY,int type, ModelHolder **boxes, ModelHolder *dach);
        virtual ~Building();

        virtual void drawBuilding(glm::mat4 P, glm::mat4 V, glm::vec4 pos);

        static float segment_heights[];/* = {0.0, 2.0, 0.0};*////height of segment under index of type, preloaded

        void print_data();

    protected:
        float global_scalar = 4.0f;

        int mX, mY; ///map coords
        float pX, pY; ///real coords considering building sizes, streets, etc
        int my_type;///zmienna odpowiadaj¹ca za typ budynku

        ModelHolder **box;

        ModelHolder *dach;
    private:

        std::vector<glm::mat4> M_vector;


        int number_of_segments;

        void generate_number_of_segments();
        virtual void generate_real_coords();
        virtual void generate_M_ground();
        void generate_M_vector();
};

#endif // BUILDING_H
