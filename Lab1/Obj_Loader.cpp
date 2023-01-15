#include "Obj_Loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareOBJIndPtr(const OBJIndex* a, const OBJIndex* b);
static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseOBJIndVal(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatVal(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string& s, char delim);

//The contructor is responsible for loading the mesh.
OBJ_Model::OBJ_Model(const std::string& fileName)
{
    hasOBJ_UVs = false;
    hasOBJ_Normals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string ln;
    if (file.is_open())
    {
        while (file.good())
        {
            getline(file, ln);

            unsigned int lnLength = ln.length();

            if (lnLength < 2)
                continue;

            const char* lineCStr = ln.c_str();

            switch (lineCStr[0])
            {
            case 'v':
                if (lineCStr[1] == 't')
                    this->OBJ_UVs.push_back(ParseOBJVector2(ln));
                else if (lineCStr[1] == 'n')
                    this->OBJ_Normals.push_back(ParseOBJVector3(ln));
                else if (lineCStr[1] == ' ' || lineCStr[1] == '\t')
                    this->OBJ_Vertices.push_back(ParseOBJVector3(ln));
                break;
            case 'f':
                CreateOBJFace(ln);
                break;
            default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

//Calculates the normals of the meshs.
void IndexedModel::CalculateNormals()
{
    for (unsigned int i = 0; i < indices.size(); i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        glm::vec3 v1 = pos[i1] - pos[i0];
        glm::vec3 v2 = pos[i2] - pos[i0];

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }

    for (unsigned int i = 0; i < pos.size(); i++)
        normals[i] = glm::normalize(normals[i]);
}

//Finds the 
IndexedModel OBJ_Model::ToIndModel()
{
    IndexedModel result;
    IndexedModel normalModel;

    unsigned int numIndices = OBJ_Indices.size();

    std::vector<OBJIndex*> indLookup;

    for (unsigned int i = 0; i < numIndices; i++)
        indLookup.push_back(&OBJ_Indices[i]);

    std::sort(indLookup.begin(), indLookup.end(), CompareOBJIndPtr);

    std::map<OBJIndex, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;

    for (unsigned int i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &OBJ_Indices[i];

        glm::vec3 currentPosition = OBJ_Vertices[currentIndex->vertexInd];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;

        if (hasOBJ_UVs)
            currentTexCoord = OBJ_UVs[currentIndex->uvInd];
        else
            currentTexCoord = glm::vec2(0, 0);

        if (hasOBJ_Normals)
            currentNormal = OBJ_Normals[currentIndex->normalInd];
        else
            currentNormal = glm::vec3(0, 0, 0);

        unsigned int normalModelIndex;
        unsigned int resultModelIndex;

        //Creates model to properly generate normals on.
        std::map<OBJIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if (it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.pos.size();

            normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.pos.push_back(currentPosition);
            normalModel.textureCoords.push_back(currentTexCoord);
            normalModel.normals.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;

        //Creates model which properly separates texture coordinates.
        unsigned int previousVertexLocation = FindLastVertexInd(indLookup, currentIndex, result);

        if (previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = result.pos.size();

            result.pos.push_back(currentPosition);
            result.textureCoords.push_back(currentTexCoord);
            result.normals.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;

        normalModel.indices.push_back(normalModelIndex);
        result.indices.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }

    if (!hasOBJ_Normals)
    {
        normalModel.CalculateNormals();

        for (unsigned int i = 0; i < result.pos.size(); i++)
            result.normals[i] = normalModel.normals[indexMap[i]];
    }

    return result;
};

//Finds the last vertex index of the mesh.
unsigned int OBJ_Model::FindLastVertexInd(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
    unsigned int startPoint = 0;
    unsigned int endPoint = indexLookup.size();
    unsigned int currentPoint = (endPoint - startPoint) / 2 + startPoint;
    unsigned int previousPoint = startPoint;

    while (currentPoint != previousPoint)
    {
        OBJIndex* testIndex = indexLookup[currentPoint];

        if (testIndex->vertexInd == currentIndex->vertexInd)
        {
            unsigned int countStartPoint = currentPoint;

            for (unsigned int i = 0; i < currentPoint; i++)
            {
                OBJIndex* possibleIndex = indexLookup[currentPoint - i];

                if (possibleIndex == currentIndex)
                    continue;

                if (possibleIndex->vertexInd != currentIndex->vertexInd)
                    break;

                countStartPoint--;
            }

            for (unsigned int i = countStartPoint; i < indexLookup.size() - countStartPoint; i++)
            {
                OBJIndex* possibleIndex = indexLookup[currentPoint + i];

                if (possibleIndex == currentIndex)
                    continue;

                if (possibleIndex->vertexInd != currentIndex->vertexInd)
                    break;
                else if ((!hasOBJ_UVs || possibleIndex->uvInd == currentIndex->uvInd)
                    && (!hasOBJ_Normals || possibleIndex->normalInd == currentIndex->normalInd))
                {
                    glm::vec3 currentPosition = OBJ_Vertices[currentIndex->vertexInd];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;

                    if (hasOBJ_UVs)
                        currentTexCoord = OBJ_UVs[currentIndex->uvInd];
                    else
                        currentTexCoord = glm::vec2(0, 0);

                    if (hasOBJ_Normals)
                        currentNormal = OBJ_Normals[currentIndex->normalInd];
                    else
                        currentNormal = glm::vec3(0, 0, 0);

                    for (unsigned int j = 0; j < result.pos.size(); j++)
                    {
                        if (currentPosition == result.pos[j]
                            && ((!hasOBJ_UVs || currentTexCoord == result.textureCoords[j])
                                && (!hasOBJ_Normals || currentNormal == result.normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }

            return -1;
        }
        else
        {
            if (testIndex->vertexInd < currentIndex->vertexInd)
                startPoint = currentPoint;
            else
                endPoint = currentPoint;
        }

        previousPoint = currentPoint;
        currentPoint = (endPoint - startPoint) / 2 + startPoint;
    }

    return -1;
}

//Creates an object face.
void OBJ_Model::CreateOBJFace(const std::string& ln)
{
    std::vector<std::string> TK = SplitString(ln, ' ');

    this->OBJ_Indices.push_back(ParseOBJInd(TK[1], &this->hasOBJ_UVs, &this->hasOBJ_Normals));
    this->OBJ_Indices.push_back(ParseOBJInd(TK[2], &this->hasOBJ_UVs, &this->hasOBJ_Normals));
    this->OBJ_Indices.push_back(ParseOBJInd(TK[3], &this->hasOBJ_UVs, &this->hasOBJ_Normals));

    if ((int)TK.size() > 4)
    {
        this->OBJ_Indices.push_back(ParseOBJInd(TK[1], &this->hasOBJ_UVs, &this->hasOBJ_Normals));
        this->OBJ_Indices.push_back(ParseOBJInd(TK[3], &this->hasOBJ_UVs, &this->hasOBJ_Normals));
        this->OBJ_Indices.push_back(ParseOBJInd(TK[4], &this->hasOBJ_UVs, &this->hasOBJ_Normals));
    }
}

//Parses the objects index.
OBJIndex OBJ_Model::ParseOBJInd(const std::string& token, bool* hasOBJ_UVs, bool* hasOBJ_Normals)
{
    unsigned int TK_Length = token.length();
    const char* TK_String = token.c_str();

    unsigned int vertIndStart = 0;
    unsigned int vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, '/');

    OBJIndex result;
    result.vertexInd = ParseOBJIndVal(token, vertIndStart, vertIndEnd);
    result.uvInd = 0;
    result.normalInd = 0;

    if (vertIndEnd >= TK_Length)
        return result;

    vertIndStart = vertIndEnd + 1;
    vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, '/');

    result.uvInd = ParseOBJIndVal(token, vertIndStart, vertIndEnd);
    *hasOBJ_UVs = true;

    if (vertIndEnd >= TK_Length)
        return result;

    vertIndStart = vertIndEnd + 1;
    vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, '/');

    result.normalInd = ParseOBJIndVal(token, vertIndStart, vertIndEnd);
    *hasOBJ_Normals = true;

    return result;
}


glm::vec3 OBJ_Model::ParseOBJVector3(const std::string& ln)
{
    unsigned int TK_Length = ln.length();
    const char* TK_String = ln.c_str();

    unsigned int vertIndStart = 2;

    while (vertIndStart < TK_Length)
    {
        if (TK_String[vertIndStart] != ' ')
            break;
        vertIndStart++;
    }

    unsigned int vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, ' ');

    float x = ParseOBJFloatVal(ln, vertIndStart, vertIndEnd);

    vertIndStart = vertIndEnd + 1;
    vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, ' ');

    float y = ParseOBJFloatVal(ln, vertIndStart, vertIndEnd);

    vertIndStart = vertIndEnd + 1;
    vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, ' ');

    float z = ParseOBJFloatVal(ln, vertIndStart, vertIndEnd);

    return glm::vec3(x, y, z);
}

glm::vec2 OBJ_Model::ParseOBJVector2(const std::string& ln)
{
    unsigned int TK_Length = ln.length();
    const char* TK_String = ln.c_str();

    unsigned int vertIndStart = 3;

    while (vertIndStart < TK_Length)
    {
        if (TK_String[vertIndStart] != ' ')
            break;
        vertIndStart++;
    }

    unsigned int vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, ' ');

    float x = ParseOBJFloatVal(ln, vertIndStart, vertIndEnd);

    vertIndStart = vertIndEnd + 1;
    vertIndEnd = FindNextCharacter(vertIndStart, TK_String, TK_Length, ' ');

    float y = ParseOBJFloatVal(ln, vertIndStart, vertIndEnd);

    return glm::vec2(x, y);
}

static bool CompareOBJIndPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->vertexInd < b->vertexInd;
}

static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char TK)
{
    unsigned int result = start;
    while (result < length)
    {
        result++;
        if (str[result] == TK)
            break;
    }

    return result;
}

static inline unsigned int ParseOBJIndVal(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatVal(const std::string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string& s, char delim)
{
    std::vector<std::string> elems;

    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int strstart = 0;
    unsigned int strEnd = 0;

    while (strEnd <= strLength)
    {
        while (strEnd <= strLength)
        {
            if (cstr[strEnd] == delim)
                break;
            strEnd++;
        }

        elems.push_back(s.substr(strstart, strEnd - strstart));
        strstart = strEnd + 1;
        strEnd = strstart;
    }

    return elems;
}