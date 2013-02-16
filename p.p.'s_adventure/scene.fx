//extern float4x4 matViewProjection;
//extern float4x4 matViewProjection = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };


//uniform extern texture gTex;
#include "define.fx"

sampler Texture0 = sampler_state
{
	Texture = <Tex>;
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
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Tex      : TEXCOORD0;
   float4 lightVec : TEXCOORD1;
   float4 shadowMapCoord : TEXCOORD2;
   float4 lightViewPos : TEXCOORD3;
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
   
   Output.lightViewPos = lightViewPos;
   
   //Output.Tex = Input.Tex;
   return( Output );
}

float4 ps_main(float2 tex: TEXCOORD0, float3 lightVec: TEXCOORD1,float2 shadowMapCoord: TEXCOORD2, float3 lightViewPos: TEXCOORD3) : COLOR0
{
	//float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	//浮点的精度导致某些本该相等的shadowMapPix和lightVec大小有差距
	//所以单纯的lightVec.r > shadowMapPix.r比较会有误差
	//此处改为lightVec.r - shadowMapPix.r  > 0.01来解决此问题
	//但是恐导致shadow平移
	//float2 smcoord;
	//smcoord.x	= 0.5*(1+lightViewPos.x);
	//smcoord.y 	= 0.5*( 1-lightViewPos.y );
	//float pix = tex2D( ShadowTex, smcoord );
	float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	if( lightVec.r - shadowMapPix.r > 0.01 )
	//if(  lightVec.r - shadowMapPix.r  > 0.001)
	//if( lightVec.r > shadowMapPix.r )
		return tex2D( Texture0, tex )-float4( 0.5f, 0.5f, 0.5f, 0.0f );
	else
		return tex2D( Texture0, tex );

	//return shadowMapPix;
}


technique main
{
    pass P0
    {
        // Specify the vertex and pixel shader associated with this pass.
        //CullMode = none;
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