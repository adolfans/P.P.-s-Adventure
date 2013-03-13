#include "StdAfx.h"
#include "MyPlayerRole.h"

#include <bitset>
using std::bitset;
#include <time.h>
MyPlayerRole::MyPlayerRole(PxScene* pScene,
											PxControllerManager* pManager,
											float height,
											SkinnedMesh* pMesh,
											MyGameSceneEntity* pEnt)
{
	con = new MyCharacterController( pScene, pManager, height, pMesh, pEnt );
	stepLength = 20.0f;
	lastAttackTime = static_cast<float>(clock())/1000.0f;
	attack = 0;//0��ʾ��������1��ʾ��һ�����������У�2��ʾ�ڶ�������3��ʾ����������
}


MyPlayerRole::~MyPlayerRole(void)
{
	delete con;
}

void MyPlayerRole::update( MSG msg )
{
	float myTimestep = 1.0f/60.0f; 
	PxVec3 moveVec( 0.0f, 0.0f, 0.0f );
	if(msg.message == WM_KEYDOWN)//����м�����
	{
		if( msg.wParam == 'Z')
		{
			long param = msg.lParam;
			bitset<sizeof(long)*8> b(param);
			if( !b[30] )//�����30λΪ0
				///::MessageBoxA( 0, "��һ��û�а��¸ü�", 0, 0 );
				//this->con->playAnim( "attack1" );
			{
				if( this->attack == 0 )
				{
					con->playAnim( "attack1" );
					this->attack = 1;
				}else if( this->attack == 1 )
				{
					con->addAnimToQueue( "attack2");
					this->attack = 2;		//��ʾ�Ѿ���attack2���붯�����У���������ʾ�Ѿ���ʼ������
				}else if( attack == 2 )
				{
					//if( con->ifCurrentAnimEnd() )
					con->addAnimToQueue( "attack3");
					this->attack = 3;
				}
				/*
				float currentTime = static_cast<float>(clock())/1000.0f;
				if( currentTime - lastAttackTime < 0.3 )
				{
					if( attackTimes == 3 )
						attackTimes = 0;
					else
						attackTimes++;
				}else
					attackTimes = 1;
				lastAttackTime = currentTime;*/
			}
		}

	}
	if( con->ifCurrentAnimEnd() )
		attack = 0;
	
	float per = 0.7071067811865475;

	if((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8000))
	{
		moveVec.x = -stepLength*per;
		moveVec.z = stepLength*per;
	}else
	if((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_DOWN) & 0x8000))
	{
		moveVec.x = -stepLength * per;
		moveVec.z = -stepLength * per;
	}else
	if((GetAsyncKeyState(VK_RIGHT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8000))
	{
		moveVec.x = stepLength * per;
		moveVec.z = stepLength * per;
	}else if((GetAsyncKeyState(VK_RIGHT) & 0x8000) && (GetAsyncKeyState(VK_DOWN) & 0x8000))
	{
		moveVec.x = stepLength * per;
		moveVec.z = -stepLength * per;
	}else
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		moveVec.x = -stepLength;//x_A += 3;
	}else
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		moveVec.x = stepLength;//x_A += 3;
	}else
	if(GetAsyncKeyState(VK_UP) & 0x8000)
	{
		moveVec.z = stepLength;//y_A += 3;
	}else
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		moveVec.z = -stepLength;//y_A -= 3;
	}

	PxControllerFilters filters( 0 );

	this->con->move( moveVec.x, moveVec.y, moveVec.z, myTimestep );
	
	this->con->update();
}
