#include "DefaultShader.hlsli"

//// シェーダーにデータ渡したいなら ** = 5 ~ 13
//cbuffer AAAAA : register(b**)
//{
//    float aaaa;
//}

Texture2D<float4> tex : register(t0);
//// テクスチャ新たに使いたいなら ** = 1 ~ 15
//Texture2D<float4> tex : register(t**);

float4 main(DefaultSpriteVSOutput input) : SV_TARGET
{
    float4 texColor = tex.Sample(wrapPointSampler,input.uv);
    float3 resultColor = texColor.rgb * color.rgb;
    float3 inverseResultColor = float3(1, 1, 1) - resultColor.rgb;

    return float4(resultColor.rgb,1);
}