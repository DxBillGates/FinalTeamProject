#include "DefaultShader.hlsli"

cbuffer TextureAnimationInfo : register(b4)
{
	// �؂蔲���T�C�Y
	float2 clipSize;
	// �؂蔲���T�C�Y�ł̐؂蔲���ʒu(0�Ȃ獶��)
	float2 pivot;
	// ���e�N�X�`���T�C�Y
	float2 textureSize;
};

DefaultMeshVSOutput main(DefaultMeshVSInput vsInput)
{
	DefaultMeshVSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(billboard, mul(modelMatrix, vsInput.pos))));
	output.normal = mul(billboard, mul(modelMatrix, float4(vsInput.normal,0))).xyz;
	output.worldPosition = mul(modelMatrix, vsInput.pos);
	float2 ancerPoint = pivot / (textureSize / clipSize);
	float2 size = clipSize / textureSize;
	output.uv = vsInput.uv * size + ancerPoint;
	output.tpos = mul(lightMatrix, mul(billboard, mul(modelMatrix, vsInput.pos)));
	return output;
}