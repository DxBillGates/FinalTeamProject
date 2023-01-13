#include "DefaultShader.hlsli"

RaymarchingVSOutput main(DefaultSpriteVSInput input)
{
	RaymarchingVSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(billboard, mul(modelMatrix, input.pos))));
	output.uv = input.uv;
	float2 uv = input.uv * 2 - 1;
	uv.y *= -1;
	float3 viewVector = mul(invViewMatrix,mul(invProjMatrix,float4(uv, 1, 1))).xyz;
	output.viewVector = viewVector;
	return output;
}