#include "DefaultShader.hlsli"

// 
Texture2D<float4> defaultLayerTexture : register(t0);
// 
Texture2D<float4> depthTexture : register(t1);
// 
Texture2D<float4> lightDepthTexture : register(t2);

Texture2D <float4> cellularNoiseTexture : register(t3);

Texture3D<float4> noiseTexture : register(t4);

cbuffer RayInfo : register(b13)
{
	float3 boundMin;
	float pad;
	float3 boundMax;
	float pad2;
	float2 perspectiveProjction;
	float DensityThreshold = 0.5;
	float DensityMultiplier = 5;
	float CloudScale = 70;
	float3 CloudOffset = float3(0, 0, 0);
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

float hash(float n)
{
	return frac(cos(n) * 114514.1919);
}

float3 random2(float3 st) {
	st = float3(dot(st, float3(127.1, 311.7,539.3)),
		dot(st, float3(269.5, 183.3,52.323)), dot(st, float3(29.5, 83.3, 52.323)));
	return -1.0 + 2.0 * frac(sin(st) * 43758.5453123);
}

//float perlinNoise(float2 st)
//{
//	float2 p = floor(st);
//	float2 f = frac(st);
//	float2 u = f * f * (3.0 - 2.0 * f);
//
//	float v00 = random2(p + float2(0, 0));
//	float v10 = random2(p + float2(1, 0));
//	float v01 = random2(p + float2(0, 1));
//	float v11 = random2(p + float2(1, 1));
//
//	return lerp(lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
//		lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
//		u.y) + 0.5f;
//}

float noise(in float3 x)
{
	float3 p = floor(x);
	float3 f = smoothstep(0.0, 1.0, frac(x));

	float n = p.x + p.y * 10.0 + p.z * 100.0;

	return lerp(
		lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
			lerp(hash(n + 10.0), hash(n + 11.0), f.x), f.y),
		lerp(lerp(hash(n + 100.0), hash(n + 101.0), f.x),
			lerp(hash(n + 110.0), hash(n + 111.0), f.x), f.y), f.z);
}

float fbm(float3 p)
{
	float3x3 m = { 0.00, 1.60, 1.20, -1.60, 0.72, -0.96, -1.20, -0.96, 1.28 };
	float f = 0.5000 * noise(p);
	p = mul(m, p);
	f += 0.2500 * noise(p);
	p = mul(m, p);
	f += 0.1666 * noise(p);
	p = mul(m, p);
	f += 0.0834 * noise(p);
	return f;
}

float LinearEyeDepth(float z)
{
	return 1.0f / (perspectiveProjction.x * z + perspectiveProjction.y);
}

float cellularnoise(float3 st, float n) {
	st *= n;

	float3 ist = floor(st);
	float3 fst = frac(st);

	float distance = 5;

	for (int y = -1; y <= 1; y++)
		for (int x = -1; x <= 1; x++) {
			float3 neighbor = float3(x, y,1);
			float3 p = 0.5 + 0.5 * sin(6.2831 * random2(ist + neighbor));

			float3 diff = neighbor + p - fst;
			distance = min(distance, length(diff));
		}

	float color = distance * 0.5;

	return color;
}

float SampleDentity(float3 position)
{
	float3 uvw = position + CloudScale * 0.001f + CloudOffset * 0.01f;
	float3 shape = noiseTexture.Sample(clampLinearSampler, uvw);
	float density = max(0, shape.r - DensityThreshold) * DensityMultiplier;
	return density;
}

float4 main(RaymarchingVSOutput input) : SV_TARGET
{
	float4 defaultLayerColor = defaultLayerTexture.Sample(clampLinearSampler,input.uv);
	float4 defaultLayerDepthColor = depthTexture.Sample(clampLinearSampler, input.uv);
	float4 shadowLayerDepthColor = lightDepthTexture.Sample(clampLinearSampler, input.uv);
	float depth = LinearEyeDepth(1 - pow(defaultLayerDepthColor,pad)) * length(input.viewVector);

	float3 rayDir = input.viewVector;
	float2 rayBoxInfo = RayBoxDst(rayDir);
	bool rayHitBox = rayBoxInfo.y > 0 && rayBoxInfo.x < depth;

	float dstTravelled = 0;
	float numSteps = 10;
	float stepSize = rayBoxInfo.y / numSteps;
	float dstLimit = min(depth - rayBoxInfo.x, rayBoxInfo.y);

	float totalDensity = 0;

	[loop]
	while(dstTravelled < dstLimit)
	{
		float3 rayPos = cameraPos.xyz + rayDir * (rayBoxInfo.x + dstTravelled);
		totalDensity += SampleDentity(rayPos) * stepSize;
		dstTravelled += stepSize;
	}

	float transmittance = exp(-totalDensity);

	//if (rayHitBox)
	//{
	//	defaultLayerColor.rgb = 0;
	//}


	return float4(defaultLayerColor.rgb * transmittance, 1);
}