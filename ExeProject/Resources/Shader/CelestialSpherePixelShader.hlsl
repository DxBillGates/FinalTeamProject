// �V���`��p�̃V�F�[�_�[
#include "DefaultShader.hlsli"

// �V���e�N�X�`����̑��z�̈ʒu��UV�ɂ����f�[�^
cbuffer SunPositionUV : register(b5)
{
	float2 sunsUV;
}

Texture2D<float4> tex : register(t0);

float EaseOutExpo(float x)
{
	return sin((x * 3.1415f) / 2);
}

float4 main(DefaultMeshVSOutput psInput) : SV_TARGET
{
	const float3 WHITE = float3(1, 1, 1);
	const float3 CREAM = float3(1, 0.92f, 0.7f);
	// �����ɂ��F�ω����z�̍����ɂ��F�ω����Ȃ���Ԃ̃f�t�H���g�J���[
	const float3 DEFAULT_SKY_COLOR = float3(0.44f, 0.66f, 1.00f);

	// ��̕\��
	float skyValue = EaseOutExpo(psInput.uv.y);
	float3 sky = lerp(DEFAULT_SKY_COLOR, WHITE, skyValue * skyValue * skyValue);

	// ���z�̕\��
	// ���z��\�����邽�߂̂������l
	float sunValue = EaseOutExpo(saturate(distance(sunsUV,psInput.uv) * 60));
	float3 sunColor = lerp(WHITE, CREAM, sunValue);
	float3 sun = sunColor * (1 - sunValue);
	float3 lightColor = float3(worldLightColor.rgb);

	float3 resultColor = sky * lightColor + sun;
	return float4(resultColor,1);
}