#include "DefaultShader.hlsli"

// 
Texture2D<float4> defaultLayerTexture : register(t0);
// 
Texture2D<float4> depthTexture : register(t1);
// 
Texture2D<float4> lightDepthTexture : register(t2);

cbuffer RayInfo : register(b13)
{
    float3 boundMin;
    float pad;
    float3 boundMax;
    float pad2;
    float samplingValue;
    float threshold;
    float2 center;
};

float3 hash3(float2 p)
{
    float3 q = float3(dot(p, float2(127.1f, 311.7f)),
					  dot(p, float2(269.5f, 183.3f)),
					  dot(p, float2(419.2f, 371.9f)));

    return frac(sin(q) * 43758.5453f);
}

float voronoi(float2 p, float u, float v)
{
    float k = 1.0f + 63.0f * pow(1.0f - v, 6.0f);

    float2 i = floor(p);
    float2 f = frac(p);
    
    float2 a = float2(0.0f, 0.0f);
    const int SAMPLING = 1;
    for (int y = -SAMPLING; y <= SAMPLING; y++)
    {
        for (int x = -SAMPLING; x <= SAMPLING; x++)
        {
            float2 g = float2(x, y);
            float3 o = hash3(i + g) * float3(u, u, 1.0f);
            float2 d = g - f + o.xy;
            float w = pow(1.0f - smoothstep(0.0f, 1.414f, length(d)), k);
            a += float2(o.z * w, w);
        }
    }
    
    float voro = (a.x / a.y);
    
    // thresholdが0の時はボロノイに一部かけている箇所があるから強制的に1にする
    if (threshold <= 0)
    {
        voro = 1;
    }
    
    // ボロノイの空間ずつ消すための処理
    return step(voro, 1.f - threshold) * (voro / threshold);
}

float LinearDepth(float depth, float near, float far)
{
    float x = 1.f - far / near;
    float y = far / near;
    float z = x / far;
    float w = y / far;
    return 1.f / (z * depth + w);
}

static const float scaleAmnt = 6.2 * 3;
static const float motionSize = 0.5f; // max 0.5
static const float s3 = 1.7320508075688772; // 2 * sin(60)
static const float i3 = 0.5773502691896258; // tan(60)
static const float2x2 tri2cart = float2x2(1.0, 0.0, -0.5, 0.5 * s3);
static const float2x2 cart2tri = float2x2(1.0, 0.0, i3, 2.0 * i3);

float3 colourForPoint(float2 uv)
{
    float3 white = 1;
    float3 black = 0;
    return lerp(white, black, uv.y);
}

#define HASHSCALE1 .1031
#define HASHSCALE3 float3(443.897, 441.423, 437.195)

float hash12(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * HASHSCALE1);
    p3 += dot(p3, p3.yzx + 19.19);
    return frac((p3.x + p3.y) * p3.z);
}

float2 hash23(float3 p3)
{
    p3 = frac(p3 * HASHSCALE3);
    p3 += dot(p3, p3.yzx + 19.19);
    return frac((p3.xx + p3.yz) * p3.zy);
}

float2 randCircle(float3 p)
{
    float2 rt = hash23(p);
    
    float r = sqrt(rt.x);
    float theta = 6.283185307179586 * rt.y;
    
    return r * float2(cos(theta), sin(theta));
    
}

float2 randCircleSpline(float2 p, float t)
{
    float t1 = floor(t);
    t -= t1;
    
    float2 pa = randCircle(float3(p, t1 - 1.0));
    float2 p0 = randCircle(float3(p, t1));
    float2 p1 = randCircle(float3(p, t1 + 1.0));
    float2 pb = randCircle(float3(p, t1 + 2.0));

    float2 m0 = 0.5 * (p1 - pa);
    float2 m1 = 0.5 * (pb - p0);

    float2 c3 = 2.0 * p0 - 2.0 * p1 + m0 + m1;
    float2 c2 = -3.0 * p0 + 3.0 * p1 - 2.0 * m0 - m1;
    float2 c1 = m0;
    float2 c0 = p0;
    
    return (((c3 * t + c2) * t + c1) * t + c0) * 0.8;
    
}

float2 triPoint(float2 p)
{
    float t0 = hash12(p);
    return mul(tri2cart, p) + motionSize * randCircleSpline(p, t0);
}

float cross(float2 p1, float2 p2, float2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(float2 pt, float2 v1, float2 v2, float2 v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = cross(pt, v1, v2);
    d2 = cross(pt, v2, v3);
    d3 = cross(pt, v3, v1);

    has_neg = (d1 < 0.0) || (d2 < 0.0) || (d3 < 0.0);
    has_pos = (d1 > 0.0) || (d2 > 0.0) || (d3 > 0.0);

    return !(has_neg && has_pos);
}

float4 main(RaymarchingVSOutput input) : SV_TARGET
{
    float4 defaultLayerColor = defaultLayerTexture.Sample(clampLinearSampler, input.uv);

    float scl = scaleAmnt;
   
    float2 p = (input.uv * 2.f - 1.f) * scl;
    
    float2 tfloor = floor(mul(cart2tri, p) + 0.5);

    float2 pts[9];

    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                pts[3 * i + j] = triPoint(tfloor + float2(i - 1, j - 1));
            }
        }
    }
    
    float2 center;
    
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            float2 t00 = pts[3 * i + j];
            float2 t10 = pts[3 * i + j + 3];
            float2 t01 = pts[3 * i + j + 1];
            float2 t11 = pts[3 * i + j + 4];
          
            if (PointInTriangle(p, t00, t10, t11)) 
                center = (t00 + t10 + t11) / 3.0;

            if (PointInTriangle(p, t00, t11, t01))
                center = (t00 + t11 + t01) / 3.0;
           
        }
    }
    
    center = center / scl;
    
    float4 col = float4(colourForPoint(center), 1.0);
    col *= step(threshold, col);
    
    return defaultLayerColor + float4(col.rgb,1);
}