// 변환행렬
float4x4    matWVP;
float		time;
bool		isAttack;
float3		inputColor;

// 텍스처
texture	tex0;

// 입력정점 형식
struct VS_INPUT
{
	float3 pos  : POSITION;
	float2 tex  : TEXCOORD0;
};

// 출력정점 형식
struct VS_OUTPUT
{
	float4 pos  : POSITION;
	float2 tex  : TEXCOORD0;
};

// VS라는 정점쉐이더 함수 선언
VS_OUTPUT VS( VS_INPUT In )
{
	// 출력 변수 초기화
	VS_OUTPUT Out = (VS_OUTPUT)0;

	if ( isAttack == true )
	{
		In.pos = In.pos + In.pos * ( sin( time ) + 1 ) / 40;
	}
	
	Out.pos = mul( float4( In.pos, 1 ), matWVP );
	Out.tex = In.tex;

	return Out;
}

// 텍스처 샘플러 상태, 
sampler Sampler = sampler_state
{
	Texture = ( tex0 ); // g_pd3dDevice->SetTexture( 0, g_pTexture );
};

float4 PS(
	float2 Tex : TEXCOORD0 ): COLOR
{
	float4 color = float4( inputColor, 1.0f );
	float4 attackColor = 0.0f;

	if ( isAttack == true )
	{
		attackColor.a = 1.0f;
		attackColor.r = 0.9f;
		attackColor.gb = 0.3f;
		attackColor = attackColor * ( ( sin( time ) + 1 ) / 2 );
	}

	return tex2D( Sampler, Tex ) / 3 + color + attackColor;
}

// MyShader 테크닉선언
technique MyShader
{
	pass P0	// 최초의 0번째 패스
	{
		Lighting = FALSE;

		// samplers
		Sampler[0] = ( Sampler );

		// shaders
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}