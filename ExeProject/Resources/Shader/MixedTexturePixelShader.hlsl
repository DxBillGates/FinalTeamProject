#include "DefaultShader.hlsli"

// gameObjectなどのレイヤー
Texture2D<float4> defaultLayerTexture : register(t0);
// パーティクルなどのエフェクト用のレイヤー
Texture2D<float4> effectLayerTexture : register(t1);

// ブルーム用ブラーレイヤー
Texture2D<float4> blurLayerTexture1 : register(t2);
Texture2D<float4> blurLayerTexture2 : register(t3);
Texture2D<float4> blurLayerTexture3: register(t4);


float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
	float4 resultColor = float4(0,0,0,1);

	float4 blur = float4(0, 0, 0, 1);
	resultColor.rgb += defaultLayerTexture.Sample(clampLinearSampler, input.uv);
	//resultColor.rgb += effectLayerTexture.Sample(clampLinearSampler, input.uv)/ 2;
	blur.rgb += blurLayerTexture1.Sample(clampLinearSampler, input.uv);
	blur.rgb += blurLayerTexture2.Sample(clampLinearSampler, input.uv);
	blur.rgb += blurLayerTexture3.Sample(clampLinearSampler, input.uv);
	blur.rgb /= 3;

	return float4((resultColor.rgb + blur.rgb),1);
}