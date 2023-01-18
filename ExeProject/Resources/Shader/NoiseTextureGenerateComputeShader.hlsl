RWTexture3D<float4> noiseTexture : register(u0);

float Random(float2 uv,float seed) 
{
	return frac(sin(dot(uv.xy, float2(12.9898, 78.233)) + seed) * 43758.5453);
}


const static  int numthread = 8;
[numthreads(numthread, numthread, numthread)]
void main( uint3 id : SV_DispatchThreadID )
{
	float4 threadColor = float4(id, 255) / 255;
	noiseTexture[id] = Random(threadColor.xy,threadColor.z);
}