//extern float4x4 matViewProjection;
//extern float4x4 matViewProjection = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };


//uniform extern texture gTex;
#include "define.fx"

float3 ambient : Ambient;
float3 diffuse : Diffuse;
float3 specular: Specular;
float3 camera  : CameraPosition;
float3 lightVector: ParallelLightVector;
float4x4 viewMatrix : ViewMatrix;
float4x4 worldMatrix: WorldMatrix;
float4x4 viewProjMatrix : ViewProjMatrix;
float4x4 reflectionMatrix : MirrorReflectionMatrix;
texture texture0		: Texture0;


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
	float2 weights	: BLENDWEIGHT0;
	int4 boneIndices : BLENDINDICES0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Tex      : TEXCOORD0;
   float4 lightViewPos : TEXCOORD1;
   float4 shadowMapCoord : TEXCOORD2;
   float3 normal	: TEXCOORD3;
   float3 viewDirection : TEXCOORD4;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
   float4 InPos = Input.Position;

	if( VertBlend == true )
	{
		float4 p = float4( 0.0f, 0.0f, 0.0f, 1.0f );
		float lastWeight = 0.0f;
		int n = NumVertInfluences -1;
		for( int i = 0; i < n; ++ i )
		{
			lastWeight += Input.weights[i];
			p += Input.weights[i]*mul( InPos,FinalTransforms[Input.boneIndices[i]] );
		}
		lastWeight = 1.0f - lastWeight;
		p+= lastWeight*mul( InPos, FinalTransforms[ Input.boneIndices[n] ] );
		p.w = 1.0f;
		InPos = p;
		Output.Position = mul( InPos, viewProjMatrix );
		//Output.Pos1 = mul( InPos, mul(reflectionMatrix,viewProjMatrix) );
	}else
	{
		Output.Position = mul( InPos, mul(worldMatrix,viewProjMatrix) );
		//Output.Pos1 = mul( InPos, mul(mul(worldMatrix,reflectionMatrix),viewProjMatrix) );
	}
   //Output.Position = Input.Position;
   
   Output.Tex = Input.Tex;
   
   float4 lightViewPoint = mul( InPos, LightViewProj );
   
   //如果这货大于在Shadow map上采样到的那货的话，它就是影子
   Output.lightViewPos = lightViewPoint.z;
   Output.lightViewPos.w = lightViewPoint.w;
   
   Output.shadowMapCoord.x = 0.5 *( lightViewPoint.x + lightViewPoint.w );
   
   Output.shadowMapCoord.y = 0.5 * ( lightViewPoint.w - lightViewPoint.y );
   
   Output.shadowMapCoord.z = 0;
   
   Output.shadowMapCoord.w = lightViewPoint.w;
   
   
   Output.normal = normalize(mul( Input.Normal,worldMatrix ));
   
   float3 fvObjectPosition = normalize(mul( InPos, worldMatrix ));
   
   Output.viewDirection = camera - fvObjectPosition;
   
   //Output.Tex = Input.Tex;
   return( Output );
}
struct PS_out{
	float4 c0 :COLOR0;
	float4 c1 :COLOR1;
};
PS_out ps_main(float2 tex: TEXCOORD0, float3 lightViewPos: TEXCOORD1,float2 shadowMapCoord: TEXCOORD2, float3 normal: TEXCOORD3, float3 viewDirection: TEXCOORD4)
{
	//float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	//浮点的精度导致某些本该相等的shadowMapPix和lightVec大小有差距
	//所以单纯的lightVec.r > shadowMapPix.r比较会有误差
	//此处改为lightVec.r - shadowMapPix.r  > 0.01来解决此问题
	//但是恐导致shadow平移

	//float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	
	// if( lightViewPos.r - shadowMapPix.r > 0.01 )
	// //if( lightViewPos.r > shadowMapPix.r )
		// color = tex2D( tex0, tex )-float4( 0.5f, 0.5f, 0.5f, 0.0f );
	// else
		// color = tex2D( tex0, tex );
	
	//return color;
	//return float4( diffuse*saturate( lightViewPos * normal )+color, 1.0 );
	//float4 specularColor;
	
	//float4 color;
	//color = lightViewPos.r - shadowMapPix.r > 0.01 ? -float4( 0.5f, 0.5f, 0.5f, 0.0f ) : 0;
	float4 color = float4( 0.5f, 0.5f, 0.5f, 0.0f )*shadowTest( shadowMapCoord, lightViewPos );

	float3 fvLightDirection = normalize( lightVector );//或者应该取反？
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
	PS_out output;
	output.c0 = float4( diffuse*saturate(dot( normalize(lightVector), normal )), 1.0f ) + color + tex2D( tex0, tex) + float4( fvTotalSpecular, 1.0f );
	output.c1 = float4( 0.0f, 0.0f, 0.0f, 1.0f );
	//return float4( diffuse*saturate(dot( normalize(lightVector), normal )), 1.0f ) + color + tex2D( tex0, tex) + float4( fvTotalSpecular, 1.0f );
	return output;
}
PS_out aa( float4 c0 : COLOR0)
{
	PS_out output;
	output.c0 = c0;
	output.c1 =float4( 0.0f, 0.0f, 0.0f, 1.0f );
	return output;
}
technique main
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        CullMode = none;
		vertexShader = compile vs_3_0 vs_main();
        pixelShader  = compile ps_3_0 ps_main();

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
	//pass P1
    //{
        // Specify the vertex and pixel shader associated with this pass.
    //    CullMode = none;
        //pixelShader  = compile ps_3_0 aa();
    //}
}