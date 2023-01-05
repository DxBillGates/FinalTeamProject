#include "DefaultShader.hlsli"

Texture2D<float4> tex : register(t0);

cbuffer SamplingInfo : register(b5)
{
	float value;
}

float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(clampPointSampler,input.uv);

	// �O���C�X�P�[���ɕϊ�
	float greyScale = (texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f);
	// �O���C�X�P�[������T���v�����O�ȏ�̒l���擾 0 or �������l�ȏ�
	float diff = step(value,greyScale);

	return float4(texColor.rgb * diff, 1.0f);
}