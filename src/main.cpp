#include <iostream>
#include <cmath>
#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glut\glut.h>
#include <3D\thresh.hpp>
#include <3D\camera.hpp>
#include <util\utility.hpp>
#include <io\lodepng.h>
#include <chrono>
#include <string>
#include <locale>
#include <codecvt>

#define WIDTH (700)
#define HEIGHT (700)
#define MIN_RADIUS (500)

#define TIME_NOW() (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()))

Thresh thresh;
CameraPtr camera;
GLuint groundTex;
std::chrono::milliseconds before(0);

bool init();
void reshape(int x, int y);
void process();
void display();
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void specialKeyboardUp(int key, int x, int y);

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("CG - TP2");

    if(!init()){
        return -1;
    }

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
	glutSpecialUpFunc(specialKeyboardUp);
    glutReshapeFunc(reshape);
	glutIdleFunc(process);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

bool init(){
	unsigned int width, height;
    std::vector<unsigned char> image;

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 300.0, 300.0, 300.0, 0.0 };

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, WIDTH/HEIGHT, 1, 100000);

    glEnable(GL_DEPTH_TEST);
	glClearDepth(10.0);

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

    try{
        camera.reset(
                     new Camera(
                                { 0.0f, 0.0f, 500.0f },
                                { 0.0f, 0.0f, 0.0f },
                                {0.0f, 1.0f, 0.0f}));
        camera->rotateV(30.0f);
        camera->rotateH(130.0f);
        thresh.load();

        /*carregar textura do chão*/
        unsigned error = lodepng::decode(image, width, height, "model\\ground.png");

        if (error)
            throw std::runtime_error("Failed to load texture!");

        glGenTextures(1, &groundTex);
        glBindTexture(GL_TEXTURE_2D, groundTex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // scale linearly when image smalled than texture

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

    }catch(std::exception& ex){
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring errorMsg = converter.from_bytes("Error: " + std::string(ex.what()));
        MessageBox(nullptr, errorMsg.c_str(), L"Falha", MB_OK);
        return false;
    }

    return true;
}

void reshape(int x, int y){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (y == 0) y=1;

    glViewport(0,0,x,y);

    GLfloat width = WIDTH,
            height = HEIGHT;
    if (x<=y)
        height *= (GLfloat)y/x;
    else
        width *= (GLfloat) x/y;

    gluPerspective(45, width/height, 1, 100000);
}

void process() {
    std::chrono::milliseconds diff = TIME_NOW() - before;

    before = TIME_NOW();
	thresh.process(diff.count() / 1000.0f);
	glutPostRedisplay();
}

void display(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //move a camera para acompanhar o modelo
    camera->setTargetPosition(thresh.getPosition());

	//limite de zoom para se aproximar do centro
	if (camera->getRadius() < MIN_RADIUS)
		camera->zoom(1.0f - (MIN_RADIUS / camera->getRadius()));

	camera->apply();

    glBindTexture(GL_TEXTURE_2D, groundTex);
	glBegin(GL_QUADS);
	glVertex3f(-1024, 0, 2048);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1024, 0, 2048);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1024, 0, -2048);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1024, 0, -2048);
	glTexCoord2f(0.0f, 1.0f);
	glEnd();

	thresh.render(0.0f);

    glutSwapBuffers();
    glFlush();
}

void keyboard(unsigned char key, int x, int y){
    switch(tolower(key)){
	case 'a': camera->rotateH(3); break;
	case 'd': camera->rotateH(-3); break;
	case 'w': camera->rotateV(-3); break;
	case 's': camera->rotateV(3); break;
	case 'z': camera->zoom(0.2f); break;
	case 'x': camera->zoom(-0.2f); break;
    }
    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y){
	switch (key){
	case GLUT_KEY_UP: thresh.walk(1); break;
	case GLUT_KEY_DOWN:	thresh.walk(-1); break;
	case GLUT_KEY_LEFT:	thresh.rotate(false); break;
	case GLUT_KEY_RIGHT:thresh.rotate(true); break;
	}
	glutPostRedisplay();
}

void specialKeyboardUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		thresh.stop();
		break;
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		thresh.stopRotating();
		break;
	}
}
