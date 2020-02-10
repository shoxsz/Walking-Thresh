#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <3D\structs.hpp>
#include <util\utility.hpp>
#include <glut\glut.h>
#include <cmath>
#include <memory>

#define PI (3.14159265358979323846)

class Camera {
public:
	Camera(const Coord& pos, const Coord& target, const Coord& up) :
		angle_v(0.0f),
		angle_h(0.0f)
	{
		set(pos, target, up);
	}

	Camera() :
		Camera({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f})
	{}

	void apply() {
		gluLookAt(pos.x, pos.y, pos.z, target.x, target.y, target.z, up.x, up.y, up.z);
	}

    void set(const Coord& pos, const Coord& target, const Coord& up){
        this->pos = pos;
        this->target = target;
        this->up = up;
        updateRadius();
    }

	void rotateH(float angle) {
		angle_h += angle*PI / 180.0f;
		//raio deve ser atualizado no caso em que as coordenadas de target/pos sejam alteradas sem manter a relação de distância dos dois(essa classe ainda não permite isso)
		//updateRadius();
		updatePos();
	}

	void rotateV(float angle) {
		angle_v += angle*PI / 180.0f;
		//raio deve ser atualizado no caso em que as coordenadas de target/pos sejam alteradas sem manter a relação de distância dos dois(essa classe ainda não permite isso)
		//updateRadius();
		updatePos();
	}

	void zoom(float zoom) {
	    zoom = 1 - zoom;
		//calcula o vetor de target-pos
		Coord dv = {pos.x - target.x, pos.y - target.y, pos.z - target.z};

		//diminui o tamanho do vetor pelo zoom
		dv.x *= zoom;
		dv.y *= zoom;
		dv.z *= zoom;

		//calcula a nova posição da camera
		pos.x = target.x + dv.x;
		pos.y = target.y + dv.y;
		pos.z = target.z + dv.z;

		//atualiza o raio
		//updateRadius();
		radius *= zoom;
	}

	/*altera a posição do alvo da camera sem alterar o sistema:
		angulos de rotação e distância até a posição da câmera*/
	void setTargetPosition(const Coord& t){
        target.x = t.x;
        target.y = t.y;
        target.z = t.z;
		updatePos();
	}

	const Coord& getTarget()const{return target;}
	const Coord& getPosition()const{return pos;}

	float getRadius()const { return radius; }
	float getAngleV()const { return angle_v; }
	float getAngleH()const { return angle_h; }

private:
	void updatePos() {
		pos.x = radius*std::sin(angle_v)*std::cos(angle_h) + target.x;
		pos.z = radius*std::sin(angle_v)*std::sin(angle_h) + target.z;
		pos.y = radius*std::cos(angle_v) + target.y;
	}

	void updateRadius() {
		radius = distance(pos, target);
	}

	Coord pos, target, up;

	float radius;//distância de pos até target
	float angle_v, angle_h;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif
