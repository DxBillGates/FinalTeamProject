#include "DefaultShader.hlsli"

cbuffer GaussFilterData : register(b5)
{
	float4 data[16];
}