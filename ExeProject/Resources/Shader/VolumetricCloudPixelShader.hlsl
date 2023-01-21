#include "DefaultShader.hlsli"

// 
Texture2D<float4> defaultLayerTexture : register(t0);
// 
Texture2D<float4> depthTexture : register(t1);
// 
Texture2D<float4> lightDepthTexture : register(t2);

cbuffer RayInfo : register(b13)
{
	float3 boundMin;
	float pad;
	float3 boundMax;
	float pad2;
	float samplingValue;
	float threshold;
	float2 center;
};

float2 RayBoxDst(float3 rayDir)
{
	float3 t0 = (boundMin - cameraPos.xyz) / rayDir;
	float3 t1 = (boundMax - cameraPos.xyz) / rayDir;
	float3 tmin = min(t0, t1);
	float3 tmax = max(t0, t1);

	float dstA = max(max(tmin.x, tmin.y), tmin.z);
	float dstB = min(tmax.x, min(tmax.y, tmax.z));

	float dstToBox = max(0, dstA);
	float dstInsideBox = max(0, dstB - dstToBox);

	return float2(dstToBox, dstInsideBox);
}

float smoothstep(float edge0, float edge1, float x)
{
	x = saturate((x - edge0) / (edge1 - edge0));
	return x * x * (3 - 2 * x);
}

float4 main(RaymarchingVSOutput input) : SV_TARGET
{
	float4 defaultLayerColor = defaultLayerTexture.Sample(clampLinearSampler,input.uv);
	float4 defaultLayerDepthColor = depthTexture.Sample(clampLinearSampler, input.uv);
	float4 shadowLayerDepthColor = lightDepthTexture.Sample(clampLinearSampler, input.uv);

	//float3 rayDir = input.viewVector;
	//float2 rayBoxInfo = RayBoxDst(rayDir);
	//bool rayHitBox = rayBoxInfo.y > 0;

	//if (rayHitBox)
	//{
	//	defaultLayerColor.rgb = 0;
	//}

	////float4 resultColor = float4(0,0,0,1);

	float4 radial = float4(0, 0, 0, 0);
	float2 pos = input.uv - center;
	float dist = length(pos);
	float factor = threshold / samplingValue * dist;

	for (int i = 0; i < (int)samplingValue; ++i)
	{
		float uvOffset = 1.0f - factor * float(i);
		radial += defaultLayerTexture.Sample(clampLinearSampler, pos * uvOffset + center);
	}

	radial /= samplingValue;

	return float4((radial.rgb),1) * color;
}