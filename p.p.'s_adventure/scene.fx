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

sampler ShadowTex = sampler_state
{
	Texture = <ShadowMap>;
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
   
   //Output.Tex = Input.Tex;
   return( Output );
}

float4 ps_main(float2 tex: TEXCOORD0, float3 lightVec: TEXCOORD1,float2 shadowMapCoord: TEXCOORD2) : COLOR0
{
	float4 shadowMapPix = tex2D( ShadowTex, shadowMapCoord );
	if(  lightVec.r - shadowMapPix.r  > 0.01)
		return float4( 0.0f, 0.0f, 0.0f, 1.0f );
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