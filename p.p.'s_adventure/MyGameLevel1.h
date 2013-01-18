#pragma once
#include "mygamelevel.h"
#include "Scene1.h"
class MyGameLevel1 :
	public MyGameLevel
{
private:
	Scene1 scene;
public:
	MyGameLevel1(void);
	~MyGameLevel1(void);
	void InitLevel(){}
	void Update(){
		scene.Render();
	}

};

