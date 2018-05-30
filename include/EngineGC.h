#ifndef ENGINEGC_H
#define ENGINEGC_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

///this is gravity end collisions engine
class EngineGC
{
    public:
        EngineGC(float** map_hei);
        virtual ~EngineGC();

        glm::vec3 collisions_simple(glm::vec3 position, glm::vec3 position_old);
        glm::vec3 collisions_advanced(glm::vec3 position, glm::vec3 position_old);

        glm::vec3 gravity_falling(glm::vec3 position);

        bool check_advanced_collisions_avalible(glm::vec3 position);




    protected:

    private:
        float time_period = 1.0f/40.0f;
        float g_gravity = 9.81;

        float s_my_height = 0;

        float v_jump;
        float v_max_jump = 30;
        float v_inc_jump = 0.1;
        float v_falling = 0;

        float **map_hei;

};

#endif // ENGINEGC_H
