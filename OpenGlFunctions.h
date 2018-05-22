#ifndef OPENGLFUNCTIONS_H_INCLUDED
#define OPENGLFUNCTIONS_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"

GLuint makeBuffer(void *data, int vertexCount, int vertexSize);

void assignVBOtoAttribute(ShaderProgram *shaderProgram,const char* attributeName, GLuint bufVBO, int vertexSize);

GLuint readTexture(const char* filename);

#endif // OPENGLFUNCTIONS_H_INCLUDED
