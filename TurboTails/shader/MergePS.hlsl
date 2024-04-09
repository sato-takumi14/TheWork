
#include "Common.hlsl"

Texture2D gTexture : register(t0);
Texture2D gBloomTexture01 : register(t1);
Texture2D gBloomTexture02 : register(t2);
Texture2D gBloomTexture03 : register(t3);
Texture2D gBloomTexture04 : register(t4);
Texture2D gBloomTexture05 : register(t5);
Texture2D gTextureDistort : register(t6);
SamplerState gSampleState : register(s0);

#define KERNEL_SIZE 5
#define SIGMA 2.0

float4 blur(float2 texcoord)
{
    float4 data = 0.0f;
    
    float2 dir = float2(0.5f, 0.5f) - texcoord;
    float len = length(dir);
    dir *= dif.x * len;
    data = gTexture.Sample(gSampleState, texcoord) * 0.19f;
    data += gTexture.Sample(gSampleState, texcoord + dir) * 0.17f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 2.0f) * 0.15f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 3.0f) * 0.13f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 4.0f) * 0.11f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 5.0f) * 0.09f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 6.0f) * 0.07f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 7.0f) * 0.05f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 8.0f) * 0.03f;
    data += gTexture.Sample(gSampleState, texcoord + dir * 9.0f) * 0.01f;
    
    return data;
}

float ACES(float data)
{
    
    float a = 2.51,
    b = 0.03,
    c = 2.43,
    d = 0.59,
    e = 0.14;
    
    return saturate((data * ((a * data) + b)) /
    data * ((data * c) + d) + c);
    
}

float3 ToneMappingOperator(float3 color)
{
    return color / (color + float3(1.0, 1.0, 1.0));
}

float gaussian(float x, float sigma)
{
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * 3.14159) * sigma);
}

float3 GaussianBlur(Texture2D tex, float2 texcoord)
{
    float2 texelSize = 1.0 / float2(1280.0, 720.0);

    float3 result = 0.0;
    float totalWeight = 0.0f;

    // ガウシアンカーネルを使ってフィルタリング
    for (int i = -KERNEL_SIZE; i <= KERNEL_SIZE; ++i)
    {
        for (int j = -KERNEL_SIZE; j <= KERNEL_SIZE; ++j)
        {
            float2 offset = float2(float(i), float(j)) * texelSize;
            float weight = gaussian(float(i), SIGMA) * gaussian(float(j), SIGMA);
            result += tex.SampleLevel(
            gSampleState, texcoord
            + offset, 0).
            rgb * weight;
            totalWeight += weight;
        }
    }
    
    return result;
    
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    outDiffuse = blur(In.TexCoord);
    outDiffuse.rgb *= In.Diffuse.rgb;
    
    float4 bloom = 0.0;
    bloom += gBloomTexture01.Sample(gSampleState, In.TexCoord);
    bloom += gBloomTexture02.Sample(gSampleState, In.TexCoord);
    bloom += gBloomTexture03.Sample(gSampleState, In.TexCoord);
    bloom += gBloomTexture04.Sample(gSampleState, In.TexCoord);
    bloom += gBloomTexture05.Sample(gSampleState, In.TexCoord);
    
    outDiffuse.rgb += bloom.rgb;
    
}