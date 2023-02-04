#include "DefaultShader.hlsli"

// gameObjectなどのレイヤー
Texture2D<float4> defaultLayerTexture : register(t0);
// パーティクルなどのエフェクト用のレイヤー
Texture2D<float4> effectLayerTexture : register(t1);

// ブルーム用ブラーレイヤー
Texture2D<float4> blurLayerTexture1 : register(t2);
Texture2D<float4> blurLayerTexture2 : register(t3);
Texture2D<float4> blurLayerTexture3: register(t4);

Texture2D<float4> depthTexture : register(t5);
Texture2D<float4> dofLayerTexture1 : register(t6);
Texture2D<float4> dofLayerTexture2: register(t7);
Texture2D<float4> effectLayerDepthTexture : register(t8);

cbuffer DofInfo : register(b13)
{
	float depth;
	float nWidth;
	float fWidth;
	float packoff;
};

float smoothstep(float edge0, float edge1, float x)
{
	x = saturate((x - edge0) / (edge1 - edge0));
	return x * x * (3 - 2 * x);
}

float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
	float4 resultColor = float4(0,0,0,1);

	float4 blur = float4(0, 0, 0, 1);
	float4 defaultLayerColor = defaultLayerTexture.Sample(clampLinearSampler, input.uv);
	float defaultLayerDepth = depthTexture.Sample(clampLinearSampler, input.uv);
	float4 effectLayerColor = effectLayerTexture.Sample(clampLinearSampler, input.uv);
	float effectLayerDepth = effectLayerDepthTexture.Sample(clampLinearSampler, input.uv);


	resultColor.rgb = defaultLayerDepth > effectLayerDepth ? effectLayerColor : defaultLayerColor;

	blur.rgb += blurLayerTexture1.Sample(clampLinearSampler, input.uv);
	blur.rgb += blurLayerTexture2.Sample(clampLinearSampler, input.uv);
	blur.rgb += blurLayerTexture3.Sample(clampLinearSampler, input.uv);
	//blur.rgb /= 3;

	//float depth = 0.05f;
	//float nWidth = 0.05f;
	//float fWidth = 0.02f;
	float4 depthTex = pow(depthTexture.Sample(clampLinearSampler, input.uv),1000);
	float near = smoothstep(0, nWidth / 2, abs(depthTex.r - depth));
	float far = smoothstep(nWidth / 2, nWidth / 2 + fWidth / 2, abs(depthTex.r - depth));
	float4 pintColor = defaultLayerColor;
	float4 nearColor = dofLayerTexture1.Sample(clampLinearSampler, input.uv);
	float4 farColor  = dofLayerTexture2.Sample(clampLinearSampler,input.uv);
	float4 dof = (1 - near) * pintColor + near * ((1 - far) * nearColor + far * farColor);

	return /*dof + */blur + resultColor;
	return float4((resultColor.rgb + blur.rgb),1);
}