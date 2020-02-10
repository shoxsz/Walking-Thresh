#include "obj_loader.hpp"
#include <util\utility.hpp>
#include <fstream>

void readVertex(std::vector<std::string>& line, ModelPtr& model);
void readVertexTexture(std::vector<std::string>& line, ModelPtr& model);
void readVertexNormal(std::vector<std::string>& line, ModelPtr& model);
void readFace(std::vector<std::string>& line, Model::Object& object);

void loadObj(const std::string& file, ModelPtr& model) {
    std::fstream stream;
    std::string line;
    std::vector<std::string> line_words;
	bool loading_model = false;

	std::string curr_obj;

    stream.open(file);

    if(!stream.is_open())
        throw std::runtime_error("Failed to open file:" + file);

	model.reset(new Model());
    while(!stream.eof()){
        std::getline(stream, line);
        split(line, ' ', line_words);

		if (line_words.empty())
			continue;

        if(line_words[0] == "o"){
			//troca o objeto atual
			curr_obj = line_words[1];
			model->addObject(line_words[1], Model::Object());
		}
		else if(line_words[0] == "v") {
			readVertex(line_words, model);
		}
		else if (line_words[0] == "vt") {
			readVertexTexture(line_words, model);
		}
		else if (line_words[0] == "vn") {
			readVertexNormal(line_words, model);
		}
		else if (line_words[0] == "f") {
			readFace(line_words, model->getObject(curr_obj));
		}
	}
}

void readVertex(std::vector<std::string>& line, ModelPtr& model){
    Coord c = {};
	model->addVertex({
		my_stof(line[1]),
		my_stof(line[2]),
		my_stof(line[3])
	});
}

void readVertexTexture(std::vector<std::string>& line, ModelPtr& model){
	model->addTexture({
		my_stof(line[1]),
		1.0f - my_stof(line[2])//coordenadas da textura estão invertidas
	});
}

void readVertexNormal(std::vector<std::string>& line, ModelPtr& model){
	model->addNormal({
        //std::stof(...)
		my_stof(line[1]),
		my_stof(line[2]),
		my_stof(line[3])
	});
}

void readFace(std::vector<std::string>& line, Model::Object& object){
    std::vector<std::string> indices;

    for(unsigned int i = 1; i < line.size(); i++){
        split(line[i], '/', indices);

        if(indices.size() != 3)
            throw std::runtime_error("Invalid vertex face parameters!");

        object.push_back
					({
						//std::stoul(...)-1,
						my_stoul(indices[0])-1,
						my_stoul(indices[1])-1,
						my_stoul(indices[2])-1
					});
    }
}
