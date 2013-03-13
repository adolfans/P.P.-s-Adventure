#include "define.fx"

float3 ambient : Ambient;
float3 diffuse : Diffuse;
float3 specular: Specular;
float3 camera  : CameraPosition;
float3 cameraVec : CameraVector;
float3 lightPos: ParallelLightVector;
float4x4 viewMatrix : ViewMatrix;
float4x4 worldMatrix: WorldMatrix;
texture texture1 : Texture1;
texture texture0 : Texture0;
texture reflectionTexture;
float2 waterspeed;

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

sampler tex1 = sampler_state
{
	Texture = <texture1>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler reflectionSampler = sampler_state
{
	Texture = <reflectionTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	//MipFilter = POINT;
	//MaxAnisotropy = 8;
	AddressU  = BORDER;
    AddressV  = BORDER;
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
   float lightVec : TEXCOORD1;
   float2 shadowMapCoord : TEXCOORD2;
   float3 normal	: NORMAL0;
   float3 viewDirection : TEXCOORD4;
   float2 scrnTexCoord	: TEXCOORD5;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, WorldViewProj );
   
   //Output.scrnTexCoord.x = 0.5 *( Output.Position.x + Output.Position.w )/Output.Position.w;
   //Output.scrnTexCoord.y = 0.5 *( Output.Position.w - Output.Position.y )/Output.Position.w;
   //Output.Position = Input.Position;
   
   Output.Tex = Input.Tex;
   
   float4 lightViewPos = mul( Input.Position, LightViewProj );
   
   //如果这货大于在Shadow map上采样到的那货的话，它就是影子
   Output.lightVec = lightViewPos.z;
   
   Output.shadowMapCoord.x = 0.5 *( lightViewPos.x + lightViewPos.w )/lightViewPos.w;
   
   Output.shadowMapCoord.y = 0.5 * ( lightViewPos.w - lightViewPos.y )/lightViewPos.w;
   
   Output.normal = normalize(mul( Input.Normal,worldMatrix ));
   
   float3 fvObjectPosition = normalize(mul( Input.Position, worldMatrix ));
   
   Output.viewDirection = camera - fvObjectPosition;
  
  
   Output.scrnTexCoord.x = 0.5 * ( 1 + Output.Position.x/Output.Position.w );
   Output.scrnTexCoord.y = 0.5 * ( 1 - Output.Position.y/Output.Position.w );
   //Output.Tex = Input.Tex;
   return( Output );
}

float4 ps_main(float2 texcoord: TEXCOORD0, float3 lightVec: TEXCOORD1,float2 shadowMapCoord: TEXCOORD2, float3 normal: NORMAL0, float3 viewDirection: TEXCOORD4, 
				float2 screenPos : VPOS, float2 scrnTexCoord: TEXCOORD5) : COLOR0
{
	//float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	//浮点的精度导致某些本该相等的shadowMapPix和lightVec大小有差距
	//所以单纯的lightVec.r > shadowMapPix.r比较会有误差
	//此处改为lightVec.r - shadowMapPix.r  > 0.01来解决此问题
	//但是恐导致shadow平移
	
	//return color;
	//return float4( diffuse*saturate( lightVec * normal )+color, 1.0 );
	//float4 specularColor;
	
	// float2 scrnTexCoord = 0;
	// scrnTexCoord.x = screenPos.x / 1024.0f;
	// scrnTexCoord.y = screenPos.y / 768.0f;
	
	float2 coord1 = texcoord;
	coord1 +=waterspeed;
	float2 coord2 = texcoord;
	coord2.x += waterspeed.x;
	coord2.y += 0.5*waterspeed.y;
	float3 mapNormal = (tex2D( tex0, coord1 ) + tex2D( tex1, coord2 )) * 0.5;	//从normal map上取得的法线
	
	mapNormal *= 2.0f;
	mapNormal -= 1.0f;
	
	float3 realNormal = mapNormal;
	realNormal.x = mapNormal.x;
	realNormal.y = mapNormal.z;
	realNormal.z = -mapNormal.y;
	
	realNormal = normalize(mul( realNormal,worldMatrix ));
   
	float3 fvLightDirection = normalize( lightPos );//或者应该取反？
	float3 fvNormal         = normalize( realNormal );
	float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
	float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); //从顶点到光的出发点的反射点的向量。
	float3 fvViewDirection  = normalize( viewDirection );
	float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );
   
	//float4 fvBaseColor      = tex2D( baseMap, Input.Texcoord );
   
	//float4 fvTotalAmbient   = fvAmbient * fvBaseColor; 
	//float4 fvTotalDiffuse   = fvDiffuse * fNDotL * fvBaseColor; 
	float3 fvTotalSpecular  =  pow( fRDotV, 10.0f) * specular;
	
	// float2 shadowmap= shadowMapCoord;
	// float4 shadowMapPix = tex2D( ShadowTex, shadowmap );
	// float4 color;
	
	// color = lightVec.r - shadowMapPix.r > 0.01 ? -float4( 0.5f, 0.5f, 0.5f, 0.0f ) : 0;

	
	//return 
	
	float4 color = float4( 0.5f, 0.5f, 0.5f, 0.0f )*shadowTest( shadowMapCoord+ 0.01* mapNormal, lightVec );

	//return float4( fvTotalSpecular, 1.0f );
	
	return tex2D( reflectionSampler, scrnTexCoord) * 0.3
	+float4( diffuse*saturate(dot( normalize(lightVec), realNormal )), 0.5f ) * 0.2 
	+ color + float4( diffuse, 1.0f ) * 0.5
	+ float4( fvTotalSpecular, 1.0f );
	//return shadowMapPix;
}


technique main
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        CullMode = none;
		AlphaBlendEnable = true;
		vertexShader = compile vs_3_0 vs_main();
        pixelShader  = compile ps_3_0 ps_main();

		// Specify the render/device states associated with this pass.
		//FillMode = wireframe;
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