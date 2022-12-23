#include "DefaultShader.hlsli"

struct SkinOutput
{
	float4 pos;
	float3 normal;
};

SkinOutput ComputeSkin(DefaultSkinMeshVSInput input)
{
	SkinOutput output = (SkinOutput)0;

	uint iBone;
	float weight;
	matrix m;
	
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	return output;
}

DefaultMeshVSOutput main(DefaultSkinMeshVSInput input)
{
	SkinOutput skinned = ComputeSkin(input);

	DefaultMeshVSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(billboard, mul(modelMatrix, skinned.pos))));
	output.uv = input.uv;
	output.normal = mul(billboard, mul(modelMatrix, float4(skinned.normal, 0))).xyz;
	output.worldPosition = mul(modelMatrix, skinned.pos);
	output.tpos = mul(lightMatrix, mul(billboard, mul(modelMatrix, input.pos)));

	return output;
}