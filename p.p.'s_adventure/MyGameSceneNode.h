#pragma once
#include "mygamenode.h"
#include "MyGameScene.h"
#include "d3dx9.h"
#include <map>
using std::map;
namespace MyGameScene{
class MyGameSceneNode :
	public MyGameNode
{

private:
	// MyGameEffect* effect; //����ӽڵ�������effect����ô���ӽڵ��effectΪ���ȣ����ڵ��effect��ȫʧЧ
	bool ifRenderShadow;			//�Ƿ����Ӱ�ӡ���������ƵĻ���������shadow map�Ľ׶ξͲ���������ڵ�
	D3DXMATRIX	localMatrix;
	D3DXMATRIX	combinedMatrix;

	D3DXMATRIX  rotateMat;		//��Ӧ��rotate����Ӧ��scale����Ӧ��pos��Ӧ�õ�local��ȥ
	D3DXMATRIX  scaleMat;		//Ҳ����rotateMat*scaleMat*posMat
	D3DXMATRIX  posMat;

	static map< string, MyGameSceneNode* > nodeMap; 
public:
	MyGameSceneNode( const char* name );
	virtual ~MyGameSceneNode(void);

	virtual void SetLocalMatrix( D3DXMATRIX& mat ){ localMatrix = mat; }

	virtual D3DXMATRIX getCombinedMatrix(){ return combinedMatrix; }
	
	
	
	virtual void ComputeCombinedMatrix(D3DXMATRIX& parentMat);
	
	virtual void move( float x, float y, float z );
	virtual void scale( float x, float y, float z );
	virtual void rotateX( float degree );
	virtual void rotateY( float degree );
	virtual void rotateZ( float degree );

	void attachEntity( MyGameSceneEntity* _ent );

	static MyGameSceneNode* getNodeByName( const char* name );

	static void	DestroyAllNodes( );
protected:
	virtual void SetCombinedMatrix( D3DXMATRIX& comMat ){ combinedMatrix = comMat;}
};

}