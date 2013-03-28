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
	virtual void Update(MSG msg)
	{
		scene.Update(msg);
	}
	virtual void Render(){
		scene.Render();
	}

};

