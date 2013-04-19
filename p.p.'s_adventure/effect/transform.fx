//extern float4x4 matViewProjection;
extern float4x4 matViewProjection = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };


uniform extern texture gTex;


sampler Texture0 = sampler_state
{
	Texture = <gTex>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
	//MaxAnisotropy = 8;
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
   float2 Tex      : TEXCOORD1;
   
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

  // Output.Position = mul( Input.Position, matViewProjection );
   
   Output.Position = Input.Position;
   
   Output.Tex = Input.Tex;
   
   //Output.Tex = Input.Tex;
   return( Output );
}

float4 ps_main(float2 tex: TEXCOORD1) : COLOR0
{   
   return tex2D( Texture0, tex );
   
}


technique TransformTech
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