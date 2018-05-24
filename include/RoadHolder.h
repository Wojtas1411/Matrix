#ifndef ROADHOLDER_H
#define ROADHOLDER_H

#include <Building.h>
#include "constants.h"

class RoadHolder : public Building
{
    public:
        RoadHolder(int mX, int mY, ModelHolder **boxes);
        virtual ~RoadHolder();

        virtual void drawBuilding(glm::mat4 P, glm::mat4 V, glm::vec4 pos);

    protected:

    private:
        glm::mat4 M;

        virtual void generate_real_coords();
        virtual void generate_M_ground();

};

#endif // ROADHOLDER_H
