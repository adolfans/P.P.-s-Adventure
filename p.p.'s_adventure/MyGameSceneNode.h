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
	// MyGameEffect* effect; //如果子节点设置了effect，那么以子节点的effect为优先，父节点的effect完全失效
	bool ifRenderShadow;			//是否绘制影子。如果不绘制的话，在生成shadow map的阶段就不绘制这个节点
	D3DXMATRIX	localMatrix;
	D3DXMATRIX	combinedMatrix;

	D3DXMATRIX  rotateMat;		//先应用rotate，再应用scale，再应用pos，应用到local中去
	D3DXMATRIX  scaleMat;		//也就是rotateMat*scaleMat*posMat
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