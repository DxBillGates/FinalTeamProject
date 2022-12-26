// 天球描画用のシェーダー
#include "DefaultShader.hlsli"

// 天球テクスチャ上の太陽の位置をUVにしたデータ
cbuffer SunPositionUV : register(b5)
{
	float2 sunsUV;
}

Texture2D<float4> tex : register(t0);

float4 main(DefaultMeshVSOutput psInput) : SV_TARGET
{
	float sun = distance(sunsUV,psInput.uv) * 40;
	float4 texColor = tex.Sample(wrapPointSampler,psInput.uv);
	texColor.xyz = sun;
	return texColor * color;
}