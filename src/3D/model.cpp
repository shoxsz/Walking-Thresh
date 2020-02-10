#include "model.hpp"

#include <windows.h>
#include <GL\GL.h>

void Model::render(bool tex, bool normal) const {
	for (auto& obj : objects) {
		render(obj.second, tex, normal);
	}
}

void Model::renderObject(const std::string& name, bool tex, bool normal) const {
	const auto& obj = objects.find(name);

	if(obj != objects.end())
		render(obj->second, tex, normal);
}

void Model::render(const Object& object, bool tex, bool normal) const {
	if (object.size() == 0)
		return;

    glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < object.size(); i++) {
        const FaceVertex& vertex = object[i];

        const Coord& coord = coords[vertex.vertexIndice];
        const TexCoord& texCoord = texCoords[vertex.textureIndice];
        const Coord& normalCoord = normals[vertex.normalIndice];

        glVertex3f(coord.x, coord.y, coord.z);
        if (tex)
            glTexCoord2f(texCoord.u, texCoord.v);
        if (normal)
            glNormal3f(normalCoord.x, normalCoord.y, normalCoord.z);
	}
    glEnd();
}
