#include "DefaultShader.hlsli"

Texture2D<float4> tex : register(t0);

float4 main(DefaultMeshVSOutput psInput) : SV_TARGET
{
	float4 texColor = tex.Sample(wrapPointSampler,psInput.uv);

	float3 lightDir = normalize(worldLightDir.xyz);
	float3 normal = normalize(psInput.normal);
	float3 eyeDir = normalize(cameraPos.xyz - psInput.worldPosition.xyz);
	float3 halfVec = normalize(-lightDir + eyeDir);

	// diffuse
	float3 diff = saturate(dot(-lightDir, normal)) * diffuse.xyz;

	// specular
	float3 spec = pow(saturate(dot(halfVec, normal)), specular.x);

	float3 intensity = (ambient.rgb + diff + spec) * worldLightColor.xyz;

	return texColor * color * float4(intensity,1.0);
}