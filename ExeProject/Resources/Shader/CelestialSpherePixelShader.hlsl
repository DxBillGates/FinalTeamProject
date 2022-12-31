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
	float3 defaultColor = float3(0.44f,0.66f,1.00f);
	float3 white = float3(1, 1, 1);
	float skyValue = EaseOutExpo(psInput.uv.y);
	float3 sky = lerp(defaultColor, white, skyValue * skyValue * skyValue);
	// 太陽を表現するためのしきい値
	float sunValue = EaseOutExpo(saturate(distance(sunsUV,psInput.uv) * 60));
	float3 cream = float3(1, 0.92f, 0.7f);
	float3 sunColor = lerp(white, cream, sunValue);
	float3 sun = sunColor * (1 - sunValue);
	float3 lightColor = float3(worldLightColor.rgb);

	//float4 texColor = tex.Sample(wrapPointSampler,psInput.uv);

	float3 resultColor = sky * lightColor + sun;
	return float4(resultColor,1);
}