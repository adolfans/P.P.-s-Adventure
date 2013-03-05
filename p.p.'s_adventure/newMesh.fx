//extern float4x4 matViewProjection;
//extern float4x4 matViewProjection = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };


//uniform extern texture gTex;
#include "define.fx"

float3 ambient : Ambient;
float3 diffuse : Diffuse;
float3 specular: Specular;
float3 camera  : CameraPosition;
texture texture0		: Texture0;
float4x4 viewProjMatrix : ViewProjMatrix;
sampler2D S0 = sampler_state
{
	Texture = <texture0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

struct VS_OUTPUT
{
	float4 pos :POSITION0;
	//float4 pos1: POSITION1;
	float2 texCoord: TEXCOORD0;
	float4 diffuse: COLOR0;
	float4 lightVec : TEXCOORD1;
	float4 shadowMapCoord : TEXCOORD2;
	float4 lightViewPos : TEXCOORD3;
};

VS_OUTPUT VertexBlend( float4 pos :POSITION0,
						float2 texCoord : TEXCOORD0,
						float2 weights	: BLENDWEIGHT0,
						int4 boneIndices : BLENDINDICES0 )
{
	VS_OUTPUT output =(VS_OUTPUT)0;
	float4 p = float4(0.0f, 0.0f, 0.0f, 1.0f );
	float lastWeight = 0.0f;
	int n = NumVertInfluences-1;
	
	for( int i =0; i < n; ++i )
	{
		lastWeight += weights[i];
		p += weights[i]*mul(pos,FinalTransforms[boneIndices[i]]);
	}
	lastWeight = 1.0f - lastWeight;
	p+= lastWeight *mul( pos, FinalTransforms[boneIndices[n]] );
	p.w = 1.0f;
	
	output.pos = mul( p, viewProjMatrix );
	
	//计算
	
	output.texCoord = texCoord;
	output.diffuse = float4( 1.0f, 1.0f, 1.0f, 1.0f );
	
	   
	float4 lightViewPos = mul( p, LightViewProj );
   
	//如果这货大于在Shadow map上采样到的那货的话，它就是影子
	output.lightVec = lightViewPos.z;
	output.lightVec.w = lightViewPos.w;
   
	output.shadowMapCoord.x = 0.5 *( lightViewPos.x + lightViewPos.w );
   
	output.shadowMapCoord.y = 0.5 * ( lightViewPos.w - lightViewPos.y );
   
	output.shadowMapCoord.z = 0;
	
	output.shadowMapCoord.w = lightViewPos.w;
   
	output.lightViewPos = lightViewPos;
   
	
	return output;
}

float4 ps_main(float2 tex: TEXCOORD0, float2 shadowMapCoord: TEXCOORD2, float3 lightVec: TEXCOORD1) : COLOR0
{  
	// float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	// //float pix = tex2D( ShadowTex, smcoord );
// /*	if( lightVec.r - shadowMapPix.r > 0.01 )
		// return float4( 0.0f, 0.0f, 0.0f, 1.0f );
	// else
		// return tex2D( S0, tex );
// */
   // //return tex2D( S0, tex );
   
	// float4 color = tex2D( S0, tex );
	// //return float4( shadowMapPix.r, shadowMapPix.r, shadowMapPix.r, 1 );
	// // if( lightVec.r - shadowMapPix.r > 0 )
		// // return color - float4( 0.5, 0.5, 0.5, 0 );
	// // else
		// // return color;
	
	// return lightVec.r-shadowMapPix.r > 0.01 ? color - float4( 0.5, 0.5, 0.5, 0 ) : color;
	return tex2D( S0, tex ) + float4( 0.5f, 0.5f, 0.5f, 0.0f )*shadowTest( shadowMapCoord, lightVec );

}

technique main
{
	pass P0
	{
		cullmode = 1;
		vertexShader = compile vs_3_0 VertexBlend();
		pixelShader  = compile ps_3_0 ps_main();
		//Sampler[0] = <S0>;
		Lighting = false;
	}
}