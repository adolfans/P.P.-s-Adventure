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
	
	//vector<int>		verticeList;//�������Ϊ���֡�\
	verticeList[0]��ʾ��һ�γ��ֵĹ����ı��εĶ���ĸ�����1��ʾ��һ�γ��ֵĹ��������εĶ���ĸ���\
	2��ʾ�ڶ��γ��ֵĹ����ı��εĶ���ĸ�����3��ʾ�����γ��ֵĹ��������εĶ���ĸ���\
																			�Դ�����
	vector<string>			materialList;
	vector<vector<int>>		verticeGroupListOfMaterial;//��materialList��һһ��Ӧ��


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

