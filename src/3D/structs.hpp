#ifndef _STRUCTS_HPP_
#define _STRUCTS_HPP_

struct Coord{
    float x, y, z;
};

struct TexCoord{
    float u, v;
};

/*V�rtice de uma face, composto pelos �ndices do v�rtice, textura
e vetor normal associados*/
struct FaceVertex{
    unsigned int vertexIndice;
    unsigned int textureIndice;
    unsigned int normalIndice;
};

#endif
