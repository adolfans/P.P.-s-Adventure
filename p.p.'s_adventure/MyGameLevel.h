#pragma once
class MyGameLevel
{	
private:
	MyGameLevel* currentLevel;

public:
	MyGameLevel(void){
		currentLevel = this;
	}
	

	virtual void InitLevel()=0;
	virtual void Update( MSG msg )=0;
	virtual void Render()=0;
	virtual ~MyGameLevel(void)
		{

	}
	void ModifyCurrentLevel(MyGameLevel* _cLevel)
	{
		currentLevel = _cLevel;
	}
	MyGameLevel* GetCurrentLevel(){
		return currentLevel;
	}
};

