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


float shadowTest( float2 shadowMapCoord, float lightVec	)//如果是阴影，返回-1,如果不是，返回0
{
	float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	
	// if( lightVec.r - shadowMapPix.r > 0.01 )
	// //if( lightVec.r > shadowMapPix.r )
		// color = tex2D( tex0, tex )-float4( 0.5f, 0.5f, 0.5f, 0.0f );
	// else
		// color = tex2D( tex0, tex );
	
	//return color;
	//return float4( diffuse*saturate( lightVec * normal )+color, 1.0 );
	//float4 specularColor;
	float jiongjiong = saturate(lightVec);
	return jiongjiong - shadowMapPix.r > 0.01 ? -1.0f : 0;
}