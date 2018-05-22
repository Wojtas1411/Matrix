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
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "ModelHolder.h"
#include "OpenGlFunctions.h"
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
        Building(int mX, int mY, unsigned short type, ModelHolder *boxes, ModelHolder *dach);
        virtual ~Building();

        drawBuilding(glm::mat4 P, glm::mat4 V);

    protected:

    private:
        int mX, mY; ///map coords
        int pX, pY; ///real coords considering building sizes, streets, etc
        ModelHolder *box;
        ModelHolder *dach;
        std::vector<glm::mat4> M_vector;
};

#endif // BUILDING_H
