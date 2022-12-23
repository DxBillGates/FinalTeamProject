#include "DefaultShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> shadowTex : register(t1);

float4 main(DefaultMeshVSOutput psInput) : SV_TARGET
{
	float3 lightDir = normalize(worldLightDir.xyz);
	float3 normal = normalize(psInput.normal);
	float3 eyeDir = normalize(cameraPos.xyz - psInput.worldPosition.xyz);
	float3 halfVec = normalize(-lightDir + eyeDir);
	float3 posFromLightVP = psInput.tpos.xyz / psInput.tpos.w;
	float2 shadowUV = (posFromLightVP.xy + float2(1, -1)) * float2(0.5f, -0.5f);
	float depthFromLight = shadowTex.Sample(clampPointSampler, shadowUV).r;
	float shadowWeight = (depthFromLight < posFromLightVP.z - 0.01f) ? 0.5f : 1;

	// diffuse
	float3 diff = saturate(dot(-lightDir, normal)) * diffuse.xyz;

	// specular
	float3 spec = pow(saturate(dot(halfVec, normal)), specular.x);

	float3 intensity = (ambient.rgb + diff + spec) * worldLightColor.xyz;
	return float4(color.rgb * intensity * shadowWeight,color.a);
}