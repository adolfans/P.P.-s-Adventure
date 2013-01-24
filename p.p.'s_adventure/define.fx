extern float4x4 WorldViewProj;
extern float4x4 FinalTransforms[35];
extern texture Tex;

extern int NumVertInfluences =4;

extern bool VertBlend = false;

extern float4x4 LightViewProj;

extern texture ShadowMap;



sampler ShadowTex = sampler_state
{
	Texture = <ShadowMap>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = BORDER;//注意寻址模式
    AddressV  = BORDER;
	BorderColor = {1.0, 1.0, 1.0, 1.0 };//这样设置的话，采样到的值就永远大于那啥……
	//也就是超出Shadow map范围的像素永远不是阴影
};

technique aaaaa
{

}