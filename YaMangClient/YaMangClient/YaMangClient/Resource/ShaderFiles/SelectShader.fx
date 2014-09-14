// 변환행렬
float4x4    matWVP;
float		time;

// 텍스처
texture	tex0;

// 입력정점 형식
struct VS_INPUT
{
	float3 pos  : POSITION;
	float4 diff : COLOR0;
	float2 tex  : TEXCOORD0;
};

// 출력정점 형식
struct VS_OUTPUT
{
	float4 pos  : POSITION;
	float4 diff : COLOR0;
	float2 tex  : TEXCOORD0;
};

// VS라는 정점쉐이더 함수 선언
VS_OUTPUT VS( VS_INPUT In )
{
	// 출력 변수 초기화
	VS_OUTPUT Out = (VS_OUTPUT)0;

	In.pos = In.pos + In.pos * ( sin( time ) + 1 ) / 50;

	float3 lightDir = float3( 1, 1, 0 );
	float lightScalar = dot( normalize( In.pos ), normalize( lightDir ) );
	float4 lightColor = 1.0;
	lightColor.xyz = 0.5 * lightScalar;

	Out.pos = mul( float4( In.pos, 1 ), matWVP );
	Out.diff = In.diff + lightColor;
	Out.tex = In.tex;

	return Out;
}

// 텍스처 샘플러 상태, 
sampler Sampler = sampler_state
{
	Texture = ( tex0 ); // g_pd3dDevice->SetTexture( 0, g_pTexture );
	MipFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	MinFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	MagFilter = LINEAR; // g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
};

float4 PS(
	float4 Diff : COLOR0,
	float2 Tex : TEXCOORD0 ): COLOR
{
	float4 color = 0.5;
	color = color * ( ( sin( time ) + 1 ) / 2 );

	return tex2D( Sampler, Tex ) / 5 + color + Diff;
}

// MyShader 테크닉선언
technique MyShader
{
	pass P0	// 최초의 0번째 패스
	{
		// g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
		Lighting = FALSE;

		// samplers
		Sampler[0] = ( Sampler );

		// shaders
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}