#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct OBJIndex
{
    unsigned int vertexInd;
    unsigned int uvInd;
    unsigned int normalInd;

    bool operator<(const OBJIndex& r) const { return vertexInd < r.vertexInd; }
};

class IndexedModel
{
public:
    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    void CalculateNormals();
};

class OBJ_Model
{
public:

    std::vector<OBJIndex> OBJ_Indices;
    std::vector<glm::vec3> OBJ_Vertices;
    std::vector<glm::vec2> OBJ_UVs;
    std::vector<glm::vec3> OBJ_Normals;
    bool hasOBJ_UVs;
    bool hasOBJ_Normals;

    OBJ_Model(const std::string& fileName);

    IndexedModel ToIndModel();

private:

    unsigned int FindLastVertexInd(const std::vector<OBJIndex*>& indLookup, const OBJIndex* currentInd, const IndexedModel& result);
    void CreateOBJFace(const std::string& line);

    glm::vec2 ParseOBJVector2(const std::string& ln);
    glm::vec3 ParseOBJVector3(const std::string& ln);
    OBJIndex ParseOBJInd(const std::string& TK, bool* hasOBJ_UVs, bool* hasOBJ_Normals);

};
#endif
