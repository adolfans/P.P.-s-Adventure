#pragma once

#include <map>
using std::map;

#include "MyGame3DDevice.h"

class testLoadObjFile
{
private:
	/*unsigned int*/ MyGameVertexBuffer* vertexBufferKey;

	/*unsigned int*/ MyGameVertexBuffer* degVertexBufferKey;

	MyGameIndexBuffer* pDegIndexBuffer;

	//MyGameTexture* pTex;
	
	//vector<int>		verticeList;//将顶点分为两种。\
	verticeList[0]表示第一次出现的构成四边形的顶点的个数，1表示第一次出现的构成三角形的顶点的个数\
	2表示第二次出现的构成四边形的顶点的个数，3表示第三次出现的构成三角形的顶点的个数\
																			以此类推
	vector<string>			materialList;
	vector<vector<int>>		verticeGroupListOfMaterial;//与materialList是一一对应的


	map<string, MyGameTexture* >	textureList;

	unsigned int	verticeNum;

	MyGame3DEffect* sceneEffect;

	unsigned int	degVerticesNum;
	unsigned int	degTrianglesNum;

public:
	testLoadObjFile( string _meshName, MyGame3DEffect* _sceneEffect );
	~testLoadObjFile(void);
	
	void render();

	int renderVertexListOfMaterial( string _materialName, vector<int> verticeList, int verticeNumDrawed );

	void updatePosition();
};

