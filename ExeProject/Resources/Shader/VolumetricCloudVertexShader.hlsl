#include "DefaultShader.hlsli"

RaymarchingVSOutput main(DefaultSpriteVSInput input)
{
	RaymarchingVSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(billboard, mul(modelMatrix, input.pos))));
	output.uv = input.uv;
	float3 viewVector = mul(invProjMatrix, float4(input.uv * 2 - 1, 0, -1));
	output.viewVector = mul(invViewMatrix, float4(viewVector,0));
	return output;
}