#pragma once

#define dDOUBLE

#include <fstream>
using namespace std;

#include <vector>
#include <ode/ode.h>


class ObjParser
{
public:
	struct VertexType
	{
		float x, y, z;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

public:
	ObjParser();
	~ObjParser();
	void Parse(const char*);

	int vertexCount, textureCount, normalCount, faceCount;

	float *dVertices;
	vector<dTriIndex[3]> dIndices;

private:
	bool result;
	char filename[256];
	char garbage;

	void GetModelFilename();
	bool ReadFileCounts();
	bool LoadDataStructures();
};

