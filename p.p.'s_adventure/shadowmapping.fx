#include "define.fx"


sampler2D S0 = sampler_state
{
	Texture = <Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 lightVec: TEXCOORD0;
};

VS_OUTPUT vertex_shader( float4 pos :POSITION0,
						float2 texCoord : TEXCOORD0,
						float2 weights	: BLENDWEIGHT0,
						int4 boneIndices : BLENDINDICES0 )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	if( VertBlend == true )//如果是顶点混合模式
	{
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
		output.Position = mul( p, LightViewProj );
	}else{
		output.Position = mul( pos, LightViewProj );
	}
		//output.lightVec = /*0.00148 * pos*/ output.Position.z * 0.01;
		output.lightVec = /*0.00148 * pos*/ output.Position.z / output.Position.w;
		return output;
}

float4 ps_main(float3 lightVec: TEXCOORD0) : COLOR0
{   
	//return float4(0,0,0,1.0f);
   //return tex2D( S0, tex );
   //return length( lightVec );
   return float4(lightVec, 1.0f );
}

technique main
{
	pass P0
	{
		ALPHABLENDENABLE   = false;
		Lighting = false;
		vertexShader = compile vs_3_0 vertex_shader();
		pixelShader  = compile ps_3_0 ps_main();
	}
}