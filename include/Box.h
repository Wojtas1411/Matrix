#ifndef BOX_H
#define BOX_H

#include <ModelHolder.h>
/*** this class inherits from ModelHolder
is supposed to read and prepare building segments from preloaded data
>textures
>obj model
so here we go ***/

class Box : public ModelHolder
{
    public:
        Box(int i);
        virtual ~Box();

    protected:

    private:
        int FIRST = 0;///defining number of possible boxes
        int LAST = 10;
};

#endif // BOX_H
