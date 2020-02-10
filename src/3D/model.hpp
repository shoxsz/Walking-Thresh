#ifndef _3D_MODEL_HPP_
#define _3D_MODEL_HPP_

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <3D\structs.hpp>

class Model{
public:
	//Um objeto é composto por um cojunto de faces nos vértices do modelo
	typedef std::vector<FaceVertex> Object;

    /*desenha o modelo completo*/
    void render(bool tex = false, bool normal = false)const;
    /*desenha o objeto com o nome indicado*/
	void renderObject(const std::string& name, bool tex = false, bool normal = false)const;

    void addVertex(const Coord& coord){
        coords.push_back(coord);
    }

    void addTexture(const TexCoord tex){
        texCoords.push_back(tex);
    }

    void addNormal(const Coord& normal){
        normals.push_back(normal);
    }

    Object& addObject(const std::string& name, const Object& object){
		objects[name] = object;
		return objects[name];
    }

	void clear() {
		coords.clear();
		texCoords.clear();
		normals.clear();
		objects.clear();
	}

	bool contains(const std::string& name) {
		return objects.find(name) != objects.end();
	}

	Object& getObject(const std::string& name) {
		return objects[name];
	}

    const std::vector<Coord>& getCoords()const{return coords;}
    const std::vector<TexCoord>& getTexCoords()const{return texCoords;}
    const std::vector<Coord>& getNormals()const{return normals;}
	const std::map<std::string, Object> & getObjects()const { return objects; }

private:
    /*renderiza o objeto indicado*/
	void render(const Object& object, bool tex = false, bool normal = false) const;

    /*buffers de coordenadas: vértices, texturas e normais*/
    std::vector<Coord> coords;
    std::vector<TexCoord> texCoords;
    std::vector<Coord> normals;

    /*vetor de objetos que compoem o modelo*/
	std::map<std::string, Object> objects;
};

typedef std::shared_ptr<Model> ModelPtr;

#endif // _3D_MODEL_HPP_
