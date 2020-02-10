#include <3D\thresh.hpp>

#include <glut\glut.h>
#include <algorithm>
#include <io\lodepng.h>

#include <iostream>
#include <chrono>

void Thresh::load() {
	GLint format;
	unsigned int width, height;
	std::vector<unsigned char> image;

	loadObj("model\\thresh.obj", model);

	/*verifica se o modelo está completo*/
	if (!model->contains("head") ||
		!model->contains("body") ||
		!model->contains("left_arm") ||
		!model->contains("left_elbow") ||
		!model->contains("left_hand") ||
		!model->contains("right_arm") ||
		!model->contains("right_elbow") ||
		!model->contains("right_hand") ||
		!model->contains("left_leg") ||
		!model->contains("left_knee") ||
		!model->contains("left_feet") ||
		!model->contains("right_leg") ||
		!model->contains("right_knee") ||
		!model->contains("right_feet") ||
		!model->contains("tools"))
		throw std::runtime_error("Invalid thresh model!");

	//inicialização dos membros
	members.resize(THRESH_MEMBER_COUNT);
	members[BODY].pivot(0.17408f, 115.74319f, -11.384f);
	members[HEAD].pivot(0.0000056f, 216.20592f, -2.71952f);
	members[LEFT_ARM].pivot(44.3101f, 163.97208f, -6.14819f);
	members[LEFT_ELBOW].pivot(54.73496f, 139.08832f, -7.52162f);
	members[LEFT_HAND].pivot(68.97387f, 106.71819f, 6.32026f);
	members[RIGHT_ARM].pivot(-43.04833f, 167.42162f, -7.36053f);
	members[RIGHT_ELBOW].pivot(-53.68162f, 138.11143f, -6.0184f);
	members[RIGHT_HAND].pivot(-68.9425f, 106.79979f, 6.24811f);
	members[LEFT_LEG].pivot(11.98622f, 112.88031f, -7.34892f);
	members[LEFT_KNEE].pivot(24.60811f, 62.38947f, -0.64953f);
	members[LEFT_FEET].pivot(26.53572f, 55.35193f, -4.52905f);
	members[RIGHT_LEG].pivot(-13.50131f, 107.45835f, -4.44652f);
	members[RIGHT_KNEE].pivot(-24.60811f, 62.38947f, -0.64953f);
	members[RIGHT_FEET].pivot(-26.53572f, 55.35194f, -4.52905f);

	members[BODY].rotate(0.0f, 0.0f, 1.0f, 0.0f);
	members[HEAD].rotate(0.0f, 0.0f, 1.0f, 0.0f);
	members[LEFT_ARM].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[LEFT_ELBOW].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[LEFT_HAND].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[RIGHT_ARM].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[RIGHT_ELBOW].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[RIGHT_HAND].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[LEFT_LEG].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[LEFT_KNEE].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[LEFT_FEET].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[RIGHT_LEG].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[RIGHT_KNEE].rotate(0.0f, 1.0f, 0.0f, 0.0f);
	members[RIGHT_FEET].rotate(0.0f, 1.0f, 0.0f, 0.0f);

	//direção inicial
	sense = 1;

	rotate_speed = 100.0f;
	walk_speed = 500.0f;

	//carregamento da textura
	unsigned error = lodepng::decode(image, width, height, "model\\thresh.png");

	if (error)
		throw std::runtime_error("Failed to load texture!");

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // scale linearly when image smalled than texture

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
}

void Thresh::process(float delta) {
	if (rotating) {
		members[BODY].rotation += (right ? -(rotate_speed*delta) : (rotate_speed*delta));
	}

	if (walking) {
		//calcula vetor de movimento através da rotação atual
		float walk_z = std::cos(members[BODY].rotation*3.1415f / 180.0f) * (walk_speed*delta);
		float walk_x = std::sin(members[BODY].rotation*3.1415f / 180.0f) * (walk_speed*delta);

		members[BODY].tz += walk_z;
		members[BODY].tx += walk_x;

		//caminha
		switch (step) {
		case 1:
			if (doStep1(delta)) {
				step = 2;
			}
			break;
		case 2:
			if (doStep2(delta, false)) {
				step = 3;
			}
			break;
		case 3:
			if (doStep2(delta, true)) {
				step = 2;
			}
			break;
		}
	}
}

void Thresh::render(float delta) {
	glBindTexture(GL_TEXTURE_2D, texId);

	glPushMatrix();
		applyTransform(members[BODY]);
		model->renderObject("body", true, true);

		//lanterna e foice
		glPushMatrix();
			applyTransform(members[TOOLS]);
			model->renderObject("tools", true, true);
		glPopMatrix();

		//cabeça
		glPushMatrix();
			applyTransform(members[HEAD]);
			model->renderObject("head", true, true);
		glPopMatrix();

		//braço esquerdo
		glPushMatrix();
			applyTransform(members[LEFT_ARM]);
			model->renderObject("left_arm", true, true);

			//cotovelo
			glPushMatrix();
				applyTransform(members[LEFT_ELBOW]);
				model->renderObject("left_elbow", true, true);

				//antebraço e mão
				glPushMatrix();
					applyTransform(members[LEFT_HAND]);
					model->renderObject("left_hand", true, true);
				glPopMatrix();
			glPopMatrix();

		glPopMatrix();

		//braço direito
		glPushMatrix();
			applyTransform(members[RIGHT_ARM]);
			model->renderObject("right_arm", true, true);

			//cotovelo
			glPushMatrix();
				applyTransform(members[RIGHT_ELBOW]);
				model->renderObject("right_elbow", true, true);

				//antebraço e mão
				glPushMatrix();
					applyTransform(members[RIGHT_HAND]);
					model->renderObject("right_hand", true, true);
				glPopMatrix();


				glPopMatrix();
		glPopMatrix();

		//perna esquerda
		glPushMatrix();
			applyTransform(members[LEFT_LEG]);
			model->renderObject("left_leg", true, true);

			//joelho
			glPushMatrix();
				applyTransform(members[LEFT_KNEE]);
				model->renderObject("left_knee", true, true);

				//canela e pé
				glPushMatrix();
					applyTransform(members[LEFT_FEET]);
					model->renderObject("left_feet", true, true);
				glPopMatrix();

			glPopMatrix();

		glPopMatrix();

		//perna direita
		glPushMatrix();
			applyTransform(members[RIGHT_LEG]);
			model->renderObject("right_leg", true, true);

			//joelho
			glPushMatrix();
				applyTransform(members[RIGHT_KNEE]);
				model->renderObject("right_knee", true, true);

				//canela e pé
				glPushMatrix();
					applyTransform(members[RIGHT_FEET]);
					model->renderObject("right_feet", true, true);
				glPopMatrix();

			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
}

bool Thresh::doStep1(float delta) {
    delta *= 4.0f;

	//mexe os braços
	members[RIGHT_ARM].rotation += 18.0f * delta;
	members[LEFT_ARM].rotation -= 18.0f * delta;

	//mexe as pernas
	members[RIGHT_LEG].rotation -= 90.0f * delta;
	members[RIGHT_KNEE].rotation += 90.0f * delta;
	members[LEFT_LEG].rotation += 45.0f * delta;

	if (members[RIGHT_LEG].rotation <= -90.0f) {
		return true;
	}

	return false;
}

bool Thresh::doStep2(float delta, bool right) {
	unsigned int forward_members_ind = (right == true ? RIGHT_ARM : LEFT_ARM);
	unsigned int backward_members_ind = (right == true ? LEFT_ARM : RIGHT_ARM);

	unsigned int forward_arm = forward_members_ind;
	unsigned int forward_elbow = forward_members_ind + 1;
	//unsigned int forward_hand = forward_members_ind + 2;
	unsigned int foward_leg = forward_members_ind + 3;
	unsigned int foward_knee = forward_members_ind + 4;
	//unsigned int foward_feet = forward_members_ind + 5;

	unsigned int backward_arm = backward_members_ind;
	unsigned int backward_elbow = backward_members_ind + 1;
	//unsigned int backward_hand = backward_members_ind + 2;
	unsigned int backard_leg = backward_members_ind + 3;
	unsigned int backard_knee = backward_members_ind + 4;
	//unsigned int backard_feet = backward_members_ind + 5;


    delta *= 4.0f;

	//braços
	members[forward_arm].rotation += 18.0f * delta;
	members[backward_arm].rotation -= 18.0f * delta;

	//pernas
	members[foward_leg].rotation -= 29.0f * delta;
	members[foward_knee].rotation += 29.0f * delta;
	members[backard_leg].rotation += 29.0f * delta;
	members[backard_knee].rotation -= 29.0f * delta;

	if (members[backard_knee].rotation <= 0)
		members[backard_knee].rotation = 0;

	if (members[foward_knee].rotation >= 90.0f)
		members[foward_knee].rotation = 90;

	if (members[foward_leg].rotation <= -90.0f) {
		return true;
	}

	return false;
}

void Thresh::applyTransform(Transform& transform) {
	glTranslatef(transform.px + transform.tx, transform.py + transform.ty, transform.pz + transform.tz);
	glRotatef(transform.rotation, transform.rx, transform.ry, transform.rz);
	glTranslatef(-(transform.px + transform.tx), -(transform.py + transform.ty), -(transform.pz + transform.tz));

	glTranslatef(transform.tx, transform.ty, transform.tz);

	glScalef(transform.sx, transform.sy, transform.sz);
}

void Thresh::rotate(bool right) {
	if (rotating == false) {
		rotating = true;
		this->right = right;
	}
}

void Thresh::stopRotating() {
	if (rotating = true) {
		rotating = false;
	}
}

void Thresh::walk(int sense) {
	if (walking == false) {

		//roda o modelo caso a direção seja oposta à atual
		if ((sense > 0 && this->sense < 0) || (sense < 0 && this->sense > 0)) {
			members[BODY].rotation += 180.0f;
		}

		//gira o cotovelos(e antebraços)
		members[LEFT_ELBOW].rotation = -90.0f;
		members[RIGHT_ELBOW].rotation = -90.0f;

		this->sense = sense;
		step = 1;
		walking = true;
	}
}

void Thresh::stop() {
	if (walking == true) {
		walking = false;
		step = 0;

		/*zera a rotação de todos os braços e pernas*/
		for (unsigned int i = LEFT_ARM; i <= RIGHT_FEET; i++) {
			members[i].rotation = 0.0f;
		}
	}
}
