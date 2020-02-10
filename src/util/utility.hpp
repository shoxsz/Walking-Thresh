#ifndef _STRING_HELPERS_HPP_
#define _STRING_HELPERS_HPP_

#include <string>
#include <vector>
#include <3D\structs.hpp>

void split(const std::string& str, char delm, std::vector<std::string>& strs);
float distance(Coord p1, Coord p2);

/*Existe um bug na versão 4.7.2 do mingw que as funções de conversão de strings
do C++ não são incluídas na biblioteca, então criei essas funções pra não ter erro
quando compilar em computadores diferentes*/

float my_stof(const std::string& str);
unsigned long my_stoul(const std::string& str);

#endif
