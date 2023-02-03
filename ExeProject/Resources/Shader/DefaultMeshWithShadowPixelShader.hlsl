#include "DefaultShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> shadowTex : register(t1);

float4 main(DefaultMeshVSOutput psInput) : SV_TARGET
{
	float3 lightDir = normalize(worldLightDir.xyz);
	float3 normal = normalize(psInput.normal);
	float3 eyeDir = normalize(cameraPos.xyz - psInput.worldPosition.xyz);
	float3 halfVec = normalize(-lightDir + eyeDir);

	float shadowWeight = 1;
	if (psInput.tpos.x < 1 && psInput.tpos.x > -1 && psInput.tpos.y < 1 && psInput.tpos.y > -1 && psInput.tpos.z < 1 && psInput.tpos.z > -1)
	{
		float3 posFromLightVP = psInput.tpos.xyz / psInput.tpos.w;
		float2 shadowUV = saturate((posFromLightVP.xy + float2(1, -1)) * float2(0.5, -0.5));
		float4 depthFromLight = shadowTex.Sample(clampPointSampler, shadowUV);
		shadowWeight = (depthFromLight.r < posFromLightVP.z - 0.0025f) ? 0.7f : 1;
	}

	// diffuse
	float3 diff = saturate(dot(-lightDir, normal)) * diffuse.xyz * worldLightColor.xyz;

	// specular
	float3 spec = pow(saturate(dot(halfVec, normal)), specular.x) * worldLightColor.xyz;

	float3 intensity = (ambient.rgb + diff + spec);

	//// ƒgƒD[ƒ“‚É‚µ‚½‚ç‹CŽ‚¿ˆ«‚­‚È‚Á‚½‚©‚çˆê’U‚â‚ß
	//// ŠÈˆÕƒgƒD[ƒ“
	//if (intensity.x > 0.95)
	//	intensity = float3(1, 1, 1);
	//else if (intensity.x > 0.2)
	//	intensity.x = float3(0.7, 0.7, 0.7);
	//else if (intensity.x > 0.1)
	//	intensity = float3(0.5, 0.5, 0.5);
	//else
	//	intensity = float3(0.2, 0.2, 0.2);
	//return float4(intensity * shadowWeight, color.a);
	return float4(color.rgb * intensity * shadowWeight,color.a);
}