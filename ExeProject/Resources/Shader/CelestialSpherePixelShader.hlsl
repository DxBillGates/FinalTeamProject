// �V���`��p�̃V�F�[�_�[
#include "DefaultShader.hlsli"

// �V���e�N�X�`����̑��z�̈ʒu��UV�ɂ����f�[�^
cbuffer SunPositionUV : register(b5)
{
	float2 sunsUV;
}

Texture2D<float4> tex : register(t0);

float4 main(DefaultMeshVSOutput psInput) : SV_TARGET
{
	float sun = distance(sunsUV,psInput.uv) * 40;
	float4 texColor = tex.Sample(wrapPointSampler,psInput.uv);
	texColor.xyz = sun;
	return texColor * color;
}