// 天球描画用のシェーダー
#include "DefaultShader.hlsli"

// 天球テクスチャ上の太陽の位置をUVにしたデータ
cbuffer SunPositionUV : register(b5)
{
	float2 sunsUV;
}

Texture2D<float4> tex : register(t0);

float EaseOutExpo(float x)
{
	return sin((x * 3.1415f) / 2);
}

float4 main(DefaultMeshVSOutput psInput) : SV_TARGET
{
	// 太陽の高さ
	float sunsHeight = 1 - sunsUV.y;
	// uvの高さ
	float uvHeight = 1 - psInput.uv.y;
	// 高さによる色変化太陽の高さによる色変化もない状態のデフォルトカラー
	float3 defaultColor = float3(0.54f,0.76f,0.86f);

	float sun = saturate(distance(sunsUV,psInput.uv) * 40);
	float4 texColor = tex.Sample(wrapPointSampler,psInput.uv);

	float3 lightColor = float3(worldLightColor.r, worldLightColor.gb);
	float3 resultColor = defaultColor * (lightColor * sun);
	return float4(resultColor,1);
}