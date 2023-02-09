#include "DefaultShader.hlsli"

// シェーダーにデータ渡したいなら ** = 5 ~ 13
cbuffer LerpValue : register(b5)
{
    float lerpValue;
}

Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);
//// テクスチャ新たに使いたいなら ** = 1 ~ 15
//Texture2D<float4> tex : register(t**);

float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
    float4 texColor = tex.Sample(wrapPointSampler,input.uv);
    float4 texColor2 = tex2.Sample(wrapPointSampler, input.uv);
    float3 resultColor = lerp(texColor.rgb,texColor2.rgb,lerpValue) * color.rgb;
    float3 inverseResultColor = float3(1, 1, 1) - resultColor.rgb;

    return float4(resultColor.rgb,1);
}