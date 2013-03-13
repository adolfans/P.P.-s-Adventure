#pragma once
class MySceneWater
{
public:
	MySceneWater(void);
	~MySceneWater(void);
	void update();
	void render();
	void setWaterSpeed( float speed );
	void setReflection( bool ifReflect );
};

