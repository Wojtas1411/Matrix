/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

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
#include "Box.h"
#include "Building.h"
#include "CityMap.h"
#include "EngineGC.h"

using namespace glm;
///skoki etc
bool EngineGC::in_air = false;

bool in_air_g = false;

bool trigger_jump = false;

///klasowe
unsigned int ModelHolder::nextTexUnit = 0;
float Building::segment_heights[] = {1.966, 2.0, 1.67, 2.34, 1.87};///height of segment under index of type, preloaded

///sekcja sterowanie
float speed_x = 0; // [radiany/s]
float speed_y = 0; // [radiany/s]

float horizontalAngle = 1.0f; // horizontal angle : toward -Z
float verticalAngle = 0.0f; // vertical angle : 0, look at the horizon

float mouseSpeed = 0.5f;

glm::vec3 position = glm::vec3( 8*500, 120, 8*500 );
glm::vec3 position_old = position;

///sekcja ekran
int global_width = 1024;
int global_height = 768;

float aspect = (float)global_width/global_height; //Stosunek szerokości do wysokości okna
///end

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (key == GLFW_KEY_R){
            position = glm::vec3( 8, 120, 8 );
            position_old = position;
        }
    }

	if(!in_air_g){
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed_y = -3.14;
		if (key == GLFW_KEY_D) speed_y = 3.14;
		if (key == GLFW_KEY_W) speed_x = -3.14;
		if (key == GLFW_KEY_S) speed_x = 3.14;
		if (key == GLFW_KEY_LEFT_SHIFT) {speed_x *= 3; speed_y *= 3;}
		if (key == GLFW_KEY_SPACE) trigger_jump = true;
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) speed_y = 0;
		if (key == GLFW_KEY_D) speed_y = 0;
		if (key == GLFW_KEY_W) speed_x = 0;
		if (key == GLFW_KEY_S) speed_x = 0;
		if (key == GLFW_KEY_LEFT_SHIFT) {speed_x /= 3; speed_y /= 3;}
		if (key == GLFW_KEY_SPACE) trigger_jump = false;
	}
	}

}

//Procedura obługi zmiany rozmiaru bufora ramki
void windowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //Obraz ma być generowany w oknie o tej rozdzielczości
    if (height!=0) {
        aspect=(float)width/(float)height; //Stosunek szerokości do wysokości okna
        global_width = width;
        global_height = height;
    } else {
        aspect=1;
    }
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.25, 0.82, 1, 1); //Czyść ekran na czarno
	glEnable(GL_DEPTH_TEST); //Włącz używanie Z-Bufora

	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury

    glfwSetFramebufferSizeCallback(window,windowResize); //Zarejestruj procedurę obsługi zmiany rozmiaru bufora ramki
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y, double deltaTime, CityMap *y, ModelHolder *ll) {
	//************Tutaj umieszczaj kod rysujący obraz******************l

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolorów

	glm::mat4 P = glm::perspective(50 * PI / 180, aspect, 0.7f, 150.0f); //Wylicz macierz rzutowania

	//glm::mat4 M = glm::mat4(1.0f);
	//M = rotate(M,(float)radians(90.0),vec3(0,1,0));
    //M = translate(M,vec3(0,0,0));

    ///mouse positioning
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    horizontalAngle += mouseSpeed * deltaTime * float(global_width/2 - xpos );
    verticalAngle   += mouseSpeed * deltaTime * float(global_height/2 - ypos );

    verticalAngle = min(verticalAngle,1.5f);
    verticalAngle = max(verticalAngle,-1.5f);

    glfwSetCursorPos(window, global_width/2, global_height/2);

    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    glm::vec3 up = glm::cross( right, direction );

    position_old = position;

    ///optionaly turn it off while jumping
    if(!in_air_g){}
    position -= direction * angle_x;
    position += right * angle_y;
    //}

    ///ograniczanie wyjscia z mapy
    position.x = max(position.x,-4.0f);
    position.z = max(position.z,-4.0f);

    position.x = min(position.x,1002*8.0f+4.0f);
    position.z = min(position.z,1002*8.0f+4.0f);

    ///***end mouse positioning***///

	glm::mat4 V = glm::lookAt(
        position,           // Camera is here
        position+direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    vec4 tmp_pos = glm::vec4(position,1);

    ///drawing section

    y->drawCityMap(P,V,tmp_pos);

	//Przerzuć tylny bufor na przedni
	glfwSwapBuffers(window);

}



int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(global_width, global_height, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	float angle_x = 0; //Kąt obrotu obiektu
	float angle_y = 0; //Kąt obrotu obiektu

	glfwSetTime(0); //Wyzeruj licznik czasu
	glfwSetCursorPos(window, global_width/2, global_height/2); //wysrodkuj mysz

	///***-----Polygon-----***///
	std::cout<<"Polygon start"<<std::endl<<std::endl;

	///tabs roads
	ModelHolder **roads = new ModelHolder*[2];
	roads[0] = new ModelHolder("objects/road2.obj","textures/roads/road0.png","textures/roads/road0.png");//skrzyżowanie
	roads[1] = new ModelHolder("objects/road1.obj","textures/roads/road1.png","textures/roads/road1.png");//droga

    ///tab of boxes
	ModelHolder **boxes = new ModelHolder*[10];
	for(int i=0;i<10;i++)boxes[i]=nullptr;
	boxes[0] = new Box(0);
	boxes[1] = new Box(1);
	boxes[2] = new Box(2);
	boxes[3] = new Box(3);
	boxes[4] = new Box(4);

	///generating roof
	dach *roof = new dach();

	///generating citymap
	CityMap *myCity = new CityMap(0,0,boxes,roof,roads);

	///generating engine for collisions and so on
	EngineGC *myEngine = new EngineGC(myCity->get_map_height());

	///res -- testzone
	ModelHolder *ff = nullptr;
	//ff = new ModelHolder("child_v4.obj","child_v4.png","child_v4.png");

    std::cout<<"Polygon end"<<std::endl<<std::endl;
	///***------end------***///

    float currenttime = 0;
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
	    //std::cout<<"X: "<<position.x<<" Y: "<<position.y<<" Z: "<<position.z
	    //<<" CHUNK X: "<<(int)(position.x+4)/8<<" Y: "<<(int)(position.z+4)/8<<" ";//<<std::endl;
	    //myCity->print_map_hei((int)position.x+4, (int)position.z+4);
	    myCity->setPosition((position.x+4)/8,(position.z+4)/8); ///zmiana pozycji renderowania


	    currenttime = glfwGetTime();
		angle_x = speed_x*currenttime;// + myEngine->x_jump_speed; //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
		angle_y = speed_y*currenttime;// + myEngine->y_jump_speed; //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
		glfwSetTime(0); //Wyzeruj licznik czasu

		if(in_air_g == true and EngineGC::in_air == false){ ///zerowanie prędkości po lądowaniu
            speed_x = 0;
            speed_y = 0;
            trigger_jump = false;
		}
		in_air_g = EngineGC::in_air; ///uzgadnianie zmiennych z silnika i sterowania

		//position = myEngine->gravity_falling(position);///grawitacja
		position = myEngine->gravity_advanced(position,trigger_jump,verticalAngle,horizontalAngle);///grawitacja i skoki

		position = myEngine->collisions_advanced(position,position_old);
		//if(position.y<100) {position = myEngine->collisions_advanced(position,position_old);}///kolizje we need an upgrade
		//else{position = myEngine->collisions_simple(position,position_old);}


		drawScene(window,angle_x,angle_y,currenttime,myCity,ff); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	std::cout<<"Exit"<<std::endl;


	///***-----DeleteSection-----***///

	delete roads[0];
	delete roads[1];
	delete[] roads;
	for(int i=0;i<10;i++){delete boxes[i];}
	delete[] boxes;
	delete roof;
	delete myCity;

	///***------end------***///

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
