#pragma once

#include <fstream>
using namespace std;


class ObjParser
{
private:
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

private:
	bool result;
	char filename[256];
	char garbage;

	void GetModelFilename();
	bool ReadFileCounts();
	bool LoadDataStructures();
};

