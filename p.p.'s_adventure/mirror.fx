//extern float4x4 matViewProjection;
//extern float4x4 matViewProjection = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };


//uniform extern texture gTex;
#include "define.fx"

float3 ambient : Ambient;
float3 diffuse : Diffuse;
float3 specular: Specular;
float3 camera  : CameraPosition;
float3 lightPos: ParallelLightVector;
float4x4 viewMatrix : ViewMatrix;
float4x4 worldMatrix: WorldMatrix;
texture texture0		: Texture0;
float4x4 mirrorMatrix;

sampler tex0 = sampler_state
{
	Texture = <texture0>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};


struct VS_INPUT 
{
	//调换顺序也可以
   float4 Position : POSITION0;
   float2 Tex      : TEXCOORD0;
   float3 Normal	:NORMAL0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Tex      : TEXCOORD0;
   float4 lightVec : TEXCOORD1;
   float4 shadowMapCoord : TEXCOORD2;
   float3 normal	: TEXCOORD3;
   float3 viewDirection : TEXCOORD4;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, WorldViewProj );
   
   //Output.Position = Input.Position;
   
   Output.Tex = Input.Tex;
   
   float4 lightViewPos = mul( Input.Position, LightViewProj );
   
   //如果这货大于在Shadow map上采样到的那货的话，它就是影子
   Output.lightVec = lightViewPos.z;
   Output.lightVec.w = lightViewPos.w;
   
   Output.shadowMapCoord.x = 0.5 *( lightViewPos.x + lightViewPos.w );
   
   Output.shadowMapCoord.y = 0.5 * ( lightViewPos.w - lightViewPos.y );
   
   Output.shadowMapCoord.z = 0;
   
   Output.shadowMapCoord.w = lightViewPos.w;
   
   
   Output.normal = normalize(mul( Input.Normal,worldMatrix ));
   
   float3 fvObjectPosition = normalize(mul( Input.Position, worldMatrix ));
   
   Output.viewDirection = camera - fvObjectPosition;
   
   
   //Output.Tex = Input.Tex;
   return( Output );
}

float4 ps_main(float2 tex: TEXCOORD0, float3 lightVec: TEXCOORD1,float2 shadowMapCoord: TEXCOORD2, float3 normal: TEXCOORD3, float3 viewDirection: TEXCOORD4) : COLOR0
{
	//float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	//浮点的精度导致某些本该相等的shadowMapPix和lightVec大小有差距
	//所以单纯的lightVec.r > shadowMapPix.r比较会有误差
	//此处改为lightVec.r - shadowMapPix.r  > 0.01来解决此问题
	//但是恐导致shadow平移

	float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	float4 color;
	if( lightVec.r - shadowMapPix.r > 0.01 )
	//if( lightVec.r > shadowMapPix.r )
		color = tex2D( tex0, tex )-float4( 0.5f, 0.5f, 0.5f, 0.0f );
	else
		color = tex2D( tex0, tex );
	
	//return color;
	//return float4( diffuse*saturate( lightVec * normal )+color, 1.0 );
	//float4 specularColor;
	
	float3 fvLightDirection = normalize( lightPos );//或者应该取反？
	float3 fvNormal         = normalize( normal );
	float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
	float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); 
	float3 fvViewDirection  = normalize( viewDirection );
	float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );
   
	//float4 fvBaseColor      = tex2D( baseMap, Input.Texcoord );
   
	//float4 fvTotalAmbient   = fvAmbient * fvBaseColor; 
	//float4 fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor; 
	float3 fvTotalSpecular  =  pow( fRDotV, 25.0f) * specular;
	
	//return float4( fvTotalSpecular, 1.0f );
	
	return float4( diffuse*saturate(dot( normalize(lightPos), normal )), 1.0f ) + color + float4( fvTotalSpecular, 1.0f );
	//return shadowMapPix;
}


technique main
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        CullMode = none;
		vertexShader = compile vs_2_0 vs_main();
        pixelShader  = compile ps_2_0 ps_main();

		// Specify the render/device states associated with this pass.
		//FillMode = Wireframe;
		/*
		AlphaBlendEnable = alphaEnable;
		SrcBlend = srcalpha;
		DestBlend = invsrcalpha;
		
		SeparateAlphaBlendEnable = false;
		SrcBlendAlpha = SrcAlpha;
		DestBlendAlpha = InvSrcAlpha;
		BlendOpAlpha = add;
		*/
		//AlphaBlendEnable = false;

    }
	
}