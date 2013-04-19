extern float4x4 WorldViewProj;
extern float4x4 FinalTransforms[35];
extern texture Tex;
extern int NumVertInfluences =4;

sampler2D S0 = sampler_state
{
	Texture = <Tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VS_OUTPUT
{
	float4 pos :POSITION0;
	float4 pos1: POSITION1;
	float2 texCoord: TEXCOORD0;
	float4 diffuse: COLOR0;
};

VS_OUTPUT VertexBlend( float4 pos :POSITION0,
						float2 texCoord : TEXCOORD0,
						float2 weights	: BLENDWEIGHT0,
						int4 boneIndices : BLENDINDICES0 )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	//float4 p = float4( 0.0f, 0.0f. 0.0f, 1.0f );
	float4 p = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lastWeight = 0.0f;
	int n = NumVertInfluences - 1;
	float4 p_t = { 0.0, 0.0, 0.0, 1.f };
	for( int i = 0; i < n + 1; ++ i )
	{
		if( i == n )
		{
			//计算最后一个权重的变换
			//lastWeight += weights[i];
			p_t = p + sign(weights[i])*mul(pos, FinalTransforms[boneIndices[i]]);
		}else
		{
			if( lastWeight <= 0.9 )
			{
			lastWeight += sign(weights[i]);
			p += sign(weights[i])*mul(pos, FinalTransforms[boneIndices[i]]);
			}
		}
	}
	//计算最后一个权重的变换
	lastWeight = sign(1.0f - lastWeight);
	p += lastWeight * mul(pos, FinalTransforms[boneIndices[n]]);

	//p = p_t;
	
	p.w = 1.0f;
	p_t.w = 1.0f;
	output.pos = mul(p, WorldViewProj );
	output.texCoord = texCoord;
	output.diffuse = float4( 1.0f, 1.0f, 1.0f, 1.0f );
	
	output.pos1 = p_t;
	
	return output;  
}

float4 ps_main(float2 tex: TEXCOORD0) : COLOR0
{   
   return tex2D( S0, tex );
}

technique VertexBlendingTech
{
	pass P0
	{
		vertexShader = compile vs_3_0 VertexBlend();
		pixelShader  = compile ps_3_0 ps_main();
		//Sampler[0] = <S0>;
		Lighting = false;
	}
}