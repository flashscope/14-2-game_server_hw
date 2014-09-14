// ��ȯ���
float4x4    matWVP;

float4 filter[9] =
{
	-1.0, -1.0, 0.0, 1.0/64,
	-1.0, 0.0, 0.0, 1.0/64,
	-1.0, 1.0, 0.0, 1.0/64,
	0.0, -1.0, 0.0, 1.0/64,
	0.0, 0.0, 0.0, 56.0/64,
	0.0, 1.0, 0.0, 1.0/64,
	1.0, -1.0, 0.0, 1.0/64,
	1.0, 0.0, 0.0, 1.0/64,
	1.0, 1.0, 0.0, 1.0/64
};

float texScaler = 1.0 / 128.0;
float texOffset = 0.0;

// �ؽ�ó
texture	tex0;

// �Է����� ����
struct VS_INPUT
{
	float3 pos  : POSITION;
	float4 diff : COLOR0;
	float2 tex  : TEXCOORD0;
};

// ������� ����
struct VS_OUTPUT
{
	float4 pos  : POSITION;
	float4 diff : COLOR0;
	float2 tex  : TEXCOORD0;
};

// VS��� �������̴� �Լ� ����
VS_OUTPUT VS( VS_INPUT In )
{
	// ��� ���� �ʱ�ȭ
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.pos = mul( float4( In.pos, 1 ), matWVP );

	float3 lightDir = float3( 0, -1, 1 );
	float lightScalar = dot( normalize( Out.pos ), normalize( lightDir ) );
	float4 lightColor = 1.0;
	lightColor.rgba = lightScalar;

	Out.diff = lightColor;
	Out.tex = In.tex;

	return Out;
}

// �ؽ�ó ���÷� ����, 
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
	float4 outColor = 0.0f;

	int i;
	for ( i = 0; i < 9; ++i )
	{
		outColor += tex2D( Sampler,
						   float2(Tex.x + filter[i].x * texScaler + texOffset, 
						   Tex.y + filter[i].y * texScaler + texOffset) ) * filter[i].w;
	}
	return tex2D( Sampler, Tex ) / 2 + outColor * Diff;
}

// MyShader ��ũ�м���
technique MyShader
{
	pass P0	// ������ 0��° �н�
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