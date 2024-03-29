struct DefaultMeshVSInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
};

struct DefaultSkinMeshVSInput
{
	float4 pos : POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

struct DefaultMeshVSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 tpos : POSITION;
	float4 worldPosition : WORLD_POSITION;
};

struct DefaultSpriteVSInput
{
	float4 pos : POSITION;
	float2 uv : UV;
};

struct DefaultSpriteVSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : UV;
};

struct RaymarchingVSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : UV;
	float3 viewVector : VIEW_VECTOR;
};

struct DefaultLineVSInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct DefaultLineVSOutput
{
	float4 svpos : SV_POSITION;
	float4 pos : POSITION;
	float4 color : COLOR;
};

static const int MAX_BONE = 64;
cbuffer ModelInfo : register(b0)
{
	matrix modelMatrix;
	matrix matSkinning[MAX_BONE];
}

cbuffer CameraInfo : register(b1)
{
	matrix viewMatrix;
	matrix projMatrix;
	matrix billboard;
	matrix lightMatrix;
	matrix invViewMatrix;
	matrix invProjMatrix;
	float4 cameraPos;
	float4 cameraDir;
}

cbuffer Material : register(b2)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 color;
}

cbuffer DirectionalLightInfo : register(b3)
{
	float4 worldLightDir;
	float4 worldLightColor;
}

float3 GetPosition(matrix mat)
{
	return float3(-mat[0][3], -mat[1][3], -mat[2][3]);
}

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);