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

unsigned int ModelHolder::nextTexUnit = 0;
float Building::segment_heights[] = {0.0, 2.0, 0.0};///height of segment under index of type, preloaded

float speed_x = 0; // [radiany/s]
float speed_y = 0; // [radiany/s]

int global_width = 1024;
int global_height = 768;

float aspect = (float)global_width/global_height; //Stosunek szerokości do wysokości okna

float horizontalAngle = 3.14f; // horizontal angle : toward -Z
float verticalAngle = 0.0f; // vertical angle : 0, look at the horizon

float mouseSpeed = 0.5f;


glm::vec3 position = glm::vec3( 0, 3, 0 );
glm::vec3 position_old = glm::vec3( 0, 3, 0 );

//Uchwyty na shadery
ShaderProgram *shaderProgram; //Wskaźnik na obiekt reprezentujący program cieniujący.

//Uchwyty na VAO i bufory wierzchołków
GLuint vao;
GLuint bufVertices; //Uchwyt na bufor VBO przechowujący tablicę współrzędnych wierzchołków
GLuint bufColors;  //Uchwyt na bufor VBO przechowujący tablicę kolorów
GLuint bufNormals; //Uchwyt na bufor VBO przechowujący tablickę wektorów normalnych

//Kostka
/*float* vertices=Models::CubeInternal::vertices;
float* colors=Models::CubeInternal::colors;
float* normals=Models::CubeInternal::normals;
int vertexCount=Models::CubeInternal::vertexCount;*/

//Czajnik
float* vertices = Models::TeapotInternal::vertices;
float* colors = Models::TeapotInternal::colors;
float* normals = Models::TeapotInternal::vertexNormals;
int vertexCount = Models::TeapotInternal::vertexCount;


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed_y = -3.14;
		if (key == GLFW_KEY_D) speed_y = 3.14;
		if (key == GLFW_KEY_W) speed_x = -3.14;
		if (key == GLFW_KEY_S) speed_x = 3.14;
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) speed_y = 0;
		if (key == GLFW_KEY_D) speed_y = 0;
		if (key == GLFW_KEY_W) speed_x = 0;
		if (key == GLFW_KEY_S) speed_x = 0;
	}
}

//Procedura obługi zmiany rozmiaru bufora ramki
void windowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //Obraz ma być generowany w oknie o tej rozdzielczości
    if (height!=0) {
        aspect=(float)width/(float)height; //Stosunek szerokości do wysokości okna
    } else {
        aspect=1;
    }
}

//Przygotowanie do rysowania pojedynczego obiektu
void prepareObject(ShaderProgram *shaderProgram) {
	//Zbuduj VBO z danymi obiektu do narysowania
	bufVertices=makeBuffer(vertices, vertexCount, sizeof(float)*4); //VBO ze współrzędnymi wierzchołków
	bufColors=makeBuffer(colors, vertexCount, sizeof(float)*4);//VBO z kolorami wierzchołków
	bufNormals=makeBuffer(normals, vertexCount, sizeof(float)*4);//VBO z wektorami normalnymi wierzchołków

	//Zbuduj VAO wiążący atrybuty z konkretnymi VBO
	glGenVertexArrays(1,&vao); //Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej

	glBindVertexArray(vao); //Uaktywnij nowo utworzony VAO

	assignVBOtoAttribute(shaderProgram,"vertex",bufVertices,4); //"vertex" odnosi się do deklaracji "in vec4 vertex;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram,"color",bufColors,4); //"color" odnosi się do deklaracji "in vec4 color;" w vertex shaderze
	assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4); //"normal" odnosi się do deklaracji "in vec4 normal;" w vertex shaderze

	glBindVertexArray(0); //Dezaktywuj VAO
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 0.4); //Czyść ekran na czarno
	glEnable(GL_DEPTH_TEST); //Włącz używanie Z-Bufora
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury
    glfwSetFramebufferSizeCallback(window,windowResize); //Zarejestruj procedurę obsługi zmiany rozmiaru bufora ramki

	shaderProgram=new ShaderProgram("vshader.vert",NULL,"fshader.frag"); //Wczytaj program cieniujący

    prepareObject(shaderProgram);
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram() {
	delete shaderProgram; //Usunięcie programu cieniującego

	glDeleteVertexArrays(1,&vao); //Usunięcie vao
	glDeleteBuffers(1,&bufVertices); //Usunięcie VBO z wierzchołkami
	glDeleteBuffers(1,&bufColors); //Usunięcie VBO z kolorami
	glDeleteBuffers(1,&bufNormals); //Usunięcie VBO z wektorami normalnymi


}

void drawObject(GLuint vao, ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM) {
	//Włączenie programu cieniującego, który ma zostać użyty do rysowania
	//W tym programie wystarczyłoby wywołać to raz, w setupShaders, ale chodzi o pokazanie,
	//że mozna zmieniać program cieniujący podczas rysowania jednej sceny
	shaderProgram->use();

	//Przekaż do shadera macierze P,V i M.
	//W linijkach poniżej, polecenie:
	//  shaderProgram->getUniformLocation("P")
	//pobiera numer slotu odpowiadającego zmiennej jednorodnej o podanej nazwie
	//UWAGA! "P" w powyższym poleceniu odpowiada deklaracji "uniform mat4 P;" w vertex shaderze,
	//a mP w glm::value_ptr(mP) odpowiada argumentowi  "mat4 mP;" TYM pliku.
	//Cała poniższa linijka przekazuje do zmiennej jednorodnej P w vertex shaderze dane z argumentu mP niniejszej funkcji
	//Pozostałe polecenia działają podobnie.
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));

	//Uaktywnienie VAO i tym samym uaktywnienie predefiniowanych w tym VAO powiązań slotów atrybutów z tablicami z danymi
	glBindVertexArray(vao);

	//Narysowanie obiektu
	glDrawArrays(GL_TRIANGLES,0,vertexCount);

	//Posprzątanie po sobie (niekonieczne w sumie jeżeli korzystamy z VAO dla każdego rysowanego obiektu)
	glBindVertexArray(0);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y, double deltaTime, CityMap *y, ModelHolder *ll) {
	//************Tutaj umieszczaj kod rysujący obraz******************l

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolorów

	glm::mat4 P = glm::perspective(50 * PI / 180, aspect, 0.8f, 100.0f); //Wylicz macierz rzutowania

	/***
	glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
		glm::vec3(0.0f, 0.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//Wylicz macierz modelu rysowanego obiektu
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle_x, glm::vec3(1, 0, 0));
	M = glm::rotate(M, angle_y, glm::vec3(0, 1, 0));
	***/

	glm::mat4 M = glm::mat4(1.0f);

	//glm::vec3 position = glm::vec3( 0, 3, 5 );

    ///mouse positioning
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    horizontalAngle += mouseSpeed * deltaTime * float(global_width/2 - xpos );//*1.0e4;
    verticalAngle   += mouseSpeed * deltaTime * float(global_height/2 - ypos );//*1.0e4;

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

    position -= direction * angle_x;
    position += right * angle_y;

    ///ograniczanie wyjscia z mapy
    position.x = max(position.x,-4.0f);
    position.z = max(position.z,-4.0f);

    position.x = min(position.x,1002*8.0f+4.0f);
    position.z = min(position.z,1002*8.0f+4.0f);

    position.y = 3.0f; ///gravity xDDD

    ///***end mouse positioning***///

    //position_old = position;

	glm::mat4 V = glm::lookAt(
        position,           // Camera is here
        position+direction, // and looks here : at the same position, plus "direction"
        up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    vec4 tmp_pos = glm::vec4(position,1);

    M = rotate(M,(float)radians(90.0),vec3(0,1,0));
    M = translate(M,vec3(-10,0,-10));


    ///drawing section

    //ll->drawObject(P,V,M,tmp_pos);

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
	std::cout<<"Polygon start"<<std::endl;

	//ModelHolder *x = new ModelHolder("test2.obj","metal.png","metal_spec.png");//testing
	///tabs roads
	ModelHolder **roads = new ModelHolder*[2];
	roads[0] = new ModelHolder("objects/road2.obj","textures/roads/road0.png","textures/roads/road0.png");//skrzyżowanie
	roads[1] = new ModelHolder("objects/road1.obj","textures/roads/road1.png","textures/roads/road1.png");//droga

    ///tab of boxes
	ModelHolder **boxes = new ModelHolder*[10];
	for(int i=0;i<10;i++)boxes[i]=nullptr;
	boxes[1] = new Box(1);

	///generating roof
	dach *roof = new dach();

	///generating citymap
	CityMap *myCity = new CityMap(0,0,boxes,roof,roads);

	///generating engine for collisions and so on
	EngineGC *myEngine = new EngineGC(myCity->get_map_height());

	///res -- testzone
	//ModelHolder *ff = new ModelHolder("wyspa_v2.obj","wyspa_v0.png","wyspa_v0.png");

    std::cout<<"Polygon end"<<std::endl;
	///***------end------***///

    float currenttime = 0;
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
	    std::cout<<"X: "<<position.x<<" Y: "<<position.y<<" Z: "<<position.z<<" CHUNK X: "<<(int)(position.x+4)/8<<" Y: "<<(int)(position.z+4)/8<<" ";//<<std::endl;
	    myCity->print_map_hei((int)position.x+4, (int)position.z+4);
	    myCity->setPosition((position.x+4)/8,(position.z+4)/8); ///zmiana pozycji renderowania

	    currenttime = glfwGetTime();
		angle_x = speed_x*glfwGetTime(); //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
		angle_y = speed_y*glfwGetTime(); //Zwiększ kąt o prędkość kątową razy czas jaki upłynął od poprzedniej klatki
		glfwSetTime(0); //Wyzeruj licznik czasu

		position = myEngine->collisions_simple(position,position_old);///kolizje

		drawScene(window,angle_x,angle_y,currenttime,myCity,nullptr); //Wykonaj procedurę rysującą
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

	freeOpenGLProgram();

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
