#ifndef _THRESH_HPP_
#define _THRESH_HPP_

#include <windows.h>
#include <gl\GL.h>

#include <3D\model.hpp>
#include <io\obj_loader.hpp>

/*Armazena informa��es sobre uma transforma��o a ser realizada:
        transla��o
        rota��o
        escala
Tamb�m armazena o piv� da rota��o, o ponto sobre o qual ela deve ocorrer
*/
struct Transform {
	float px, py, pz;		//piv� da transforma��o(usado na rota��o)
	float tx, ty, tz;
	float rotation, rx, ry, rz;
	float sx, sy, sz;

	Transform() {
		px = py = pz = tx = ty = tz = rotation = rx = ry = rz = 0.0f;
		sx = sy = sz = 1.0f;
	}

	void pivot(float x, float y, float z) {
		px = x;
		py = y;
		pz = z;
	}

	void translate(float x, float y, float z) {
		tx = x;
		ty = y;
		tz = z;
	}

	void rotate(float angle, float x, float y, float z) {
		rotation = angle;
		rx = x;
		ry = y;
		rz = z;
	}

	void scale(float x, float y, float z) {
		sx = x;
		sy = y;
		sz = z;
	}
};

/*A classe do modelo 3D apenas guarda a representa��o de um modelo,
ela n�o tem no��o de hier�rquia, a classe Thresh representa o boneco
e tem no��o da hier�rquia do modelo*/
class Thresh {
public:
    /*Lista de membros do modelo*/
	enum ThreshMember{
		HEAD = 0,
		BODY,
		LEFT_ARM,
		LEFT_ELBOW,
		LEFT_HAND,
		LEFT_LEG,
		LEFT_KNEE,
		LEFT_FEET,
		RIGHT_ARM,
		RIGHT_ELBOW,
		RIGHT_HAND,
		RIGHT_LEG,
		RIGHT_KNEE,
		RIGHT_FEET,
		TOOLS,
		THRESH_MEMBER_COUNT
	};

	Thresh(){}
	~Thresh() { glDeleteTextures(1, &texId); }

	void load();
	/*processa as anima��es atuais*/
	void process(float delta);
	/*renderiza o modelo hierarquicamente*/
	void render(float delta);

	/*rotaciona o modelo na dire��o indicada*/
	void rotate(bool right);
	void stopRotating();

	//anda na dire��o indicada: sense > 0 -> frente | sense < 0 -> tr�s
	void walk(int sense);
	void stop();

	float getRotation()const { return members[BODY].rotation; }
	Coord getPosition()const {
		return {
				members[BODY].tx + members[HEAD].px,
				members[BODY].ty + members[HEAD].py,
				members[BODY].tz + members[HEAD].pz };
	}

private:
	/*aplica a transforma��o*/
	void applyTransform(Transform& transform);

	//d� o primero passo(parado -> caminhar)
	bool doStep1(float delta);
	//d� o segundo passo com a perna indicada
	bool doStep2(float delta, bool right);

	/*Vetor de transforma��es para cada membro*/
	std::vector<Transform> members;
	ModelPtr model;

	/*Textura n�o est� sendo aplicada muito bem :/*/
	GLuint texId;

	//ROTATE vars
	bool rotating;
	bool right;
	float rotate_speed; //graus por segundo

	//WALK vars
	bool walking;
	int sense;
	float walk_speed; //unidades por segundo

	//ANIMA��O

	//a sequencia de anima��o �: step 1 -> (step 2 <-> step 3)
	int step;
    float forward_leg_rot_speed;
    float backward_leg_rot_speed;
};

#endif
