#pragma once
#include <d3dx9.h>
#include <map>
using std::map;
#include "mygamenode.h"
#include "MyGameScene.h"
namespace MyGameScene{
class MyGameSceneNode :
	public MyGameNode
{
	friend class MyGameSceneManager;
private:
	// MyGameEffect* effect; //����ӽڵ�������effect����ô���ӽڵ��effectΪ���ȣ����ڵ��effect��ȫʧЧ
	bool ifRenderShadow;			//�Ƿ����Ӱ�ӡ���������ƵĻ���������shadow map�Ľ׶ξͲ���������ڵ�
	D3DXMATRIX	localMatrix;
	D3DXMATRIX	combinedMatrix;

	D3DXMATRIX  rotateMat;		//��Ӧ��rotate����Ӧ��scale����Ӧ��pos��Ӧ�õ�local��ȥ
	D3DXMATRIX  scaleMat;		//Ҳ����rotateMat*scaleMat*posMat
	D3DXMATRIX  posMat;

	static map< string, MyGameSceneNode* > nodeMap; 
protected:
	MyGameSceneNode( const char* name );
	virtual ~MyGameSceneNode(void);
public:

	virtual void SetLocalMatrix( D3DXMATRIX& mat ){ localMatrix = mat; }

	virtual const D3DXMATRIX& getCombinedMatrix(){ return combinedMatrix; }
	
	
	
	virtual void ComputeCombinedMatrix( const D3DXMATRIX& parentMat );
	
	virtual void move( float x, float y, float z );
	virtual void scale( float x, float y, float z );
	virtual void rotateX( float angle );
	virtual void rotateY( float angle );
	virtual void rotateZ( float angle );

	virtual void setPosition( float x, float y, float z );

	virtual void setRotateMatrix( const D3DXMATRIX& rotatemat );

	virtual void setRotationAngleY( float angle );

	void attachEntity( MyGameSceneEntity* _ent );

	static MyGameSceneNode* getNodeByName( const char* name );

	static void	destroyAllNodes( );
	
	virtual void setTranslationMatrix( D3DXMATRIX& trans )
	{
		localMatrix = rotateMat*scaleMat*trans;
	}
protected:
	virtual void SetCombinedMatrix( D3DXMATRIX& comMat ){ combinedMatrix = comMat;}
};

}