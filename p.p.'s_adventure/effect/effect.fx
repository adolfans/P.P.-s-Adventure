//float4x4 matViewProjection;

struct VS_INPUT 
{
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

   //Output.Position = mul( Input.Position, matViewProjection );
   
   Output.Position = Input.Position;
   Output.Tex = Input.Tex;
   
   Output.Position.xy = sign(Input.Position.xy);
   //Output.Position.x = sign( Input.Position.x );
   //Output.Position.y = sign( Input.Position.y );
   //Output.Position.z = 0;
   Output.Position.w = 1.0f;
   Output.Tex = (float2(Output.Position.x, -Output.Position.y) + 1.0f)/2.0f;
   
   //Output.Tex = Input.Tex;
   return( Output );
   
}


uniform extern texture gTex;
uniform extern texture gTex_t;

sampler Texture0 = sampler_state
{
	Texture = <gTex>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler Texture1 = sampler_state
{
	Texture = <gTex_t>;
	MinFilter = Anisotropic;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	MaxAnisotropy = 8;
	AddressU  = WRAP;
    AddressV  = WRAP;

};


// this texture is 490*306
float2 texSize = { 800, 600 };
float sigma = 1.0f;
extern int mode = 1;

float Gaussian( float sigma, float x )
{
   float pi = 3.1415927;
   float e  = 2.7;
   //float rs = (1/(sqrt(2.0*pi)*sigma))*e^(-1*x*x/(2*sigma*sigma)));
   //float rs = ((1/(sqrt(2.0*pi)*sigma))*pow(-1.0*x*x/(2.0*sigma*sigma),e));
   float aaa = (float)pow(e,-1.0*x*x/(2.0*sigma*sigma));
   float rs = (1.0/(sqrt(2.0*pi)*sigma))*aaa;
   return rs;
}

float3 blurX( sampler2D texSampler, float2 tex, float sigma, float pixelx )
{
   sigma = abs(sigma);
   float3 color = 0;
   //计算到3sigma
   color+=tex2D( texSampler, tex )*Gaussian( sigma, 0 );
   for( int i = 1; i < 3*sigma+1; i++ )
   {   
      color += Gaussian(sigma, i)*( tex2D(texSampler, float2(tex.x-i*pixelx, tex.y))+tex2D(texSampler,float2(tex.x+i*pixelx,tex.y)) );
   }
   return color;
}


float4 ps_main(float2 tex: TEXCOORD1) : COLOR0
{   
   float2 pixel = 0;
   pixel.x = 1.0f/texSize.x;
   pixel.y = 1.0f/texSize.y;
   //return( float4( 1.0f, 0.0f, 0.0f, 1.0f ) );
   //return tex2D( Texture0, tex );
   //int tex1 = tex;
   
   float3x3 smoothMatrix = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
   smoothMatrix*=1.0f/16.0f;

   float3 color=tex2D( Texture0, tex )*Gaussian( sigma, 0 );
   for( int i = 1; i < 7; i++ )
   {   
      color += Gaussian(sigma, i)*( tex2D(Texture0, float2(tex.x-i*pixel.x, tex.y))+tex2D(Texture0,float2(tex.x+i*pixel.x,tex.y)) );
      //color += Gaussian(sigma, i)*( tex2D(Texture0, tex-pixel)+tex2D(Texture0, tex+pixel) );
   }

   return float4( color, 1.0f );
   
}


float4 ps_main_2(float2 Tex: TEXCOORD1) : COLOR0
{   
   
   float2 pixel = 0;
   pixel.x = 1.0f/texSize.x;
   pixel.y = 1.0f/texSize.y;
   
   float3 color=tex2D( Texture1, Tex )*Gaussian( sigma, 0 );
   for( int i = 1; i < 7; i++ )
   {   
      float delta = (float)i*pixel.y;
      color += Gaussian(sigma, i)*( tex2D(Texture1, float2(Tex.x, Tex.y-delta))+tex2D(Texture1,float2(Tex.x,Tex.y+delta)) );
       
      //color += Gaussian(sigma, i)*( tex2D(Texture1, Tex-pixel)+tex2D(Texture1, Tex+pixel) );
   
   }
   
   
   float3 color2 = tex2D( Texture0, Tex );
   
   if(mode==1){
   //Addition
   color+=color2;}
   else if(mode ==2 ){
   //减淡
   color = color/(1-color2);}
   else if(mode==3){
   //柔光(1-x2)*x1^2 + x2*(1-(1-x2)^2)
   color = (1-color2)*(color*color) + color2*(1-(1-color2)*(1-color2));
   }else if(mode==4){
   //Grain Merge: f(x1,x2, 21) = CLAMP(x1+x2-0.5)增益合并
   color = saturate( color+color2-0.5);
   }else if(mode==5)//screen
   color = 1 - (1-color)*(1-color2);
   
   //硬光if x2 < 0.5 then 2*x1*x2 else 1-2*(1-x1)(1-x2)
   //懒了。。。
   
   return float4(color,1.0f);
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
		// FillMode = Wireframe;
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
	
	pass P1
	{
		vertexShader = compile vs_2_0 vs_main();
		pixelShader = compile ps_2_a ps_main_2();
	}
}
