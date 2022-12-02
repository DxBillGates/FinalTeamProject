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

DefaultSpriteVSOutput main(DefaultSpriteVSInput input)
{
	DefaultSpriteVSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(billboard, mul(modelMatrix, input.pos))));

	float2 ancerPoint = pivot / (textureSize / clipSize);
	float2 size = clipSize / textureSize;
	output.uv = input.uv * size + ancerPoint;
	return output;
}