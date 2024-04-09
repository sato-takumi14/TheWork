
#include"Common.hlsl"

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

#define KERNEL_SIZE 5
#define SIGMA 2.0

static const float weights[9] =
{
    1, 2, 1,
    2, 4, 2,
    1, 2, 1
};

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
            gSamplerState, texcoord
            + offset, 0).
            rgb * weight;
            totalWeight += weight;
        }
    }
    
    return result;
    
}

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = gTexture.Sample(gSamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;    
    
    float4 bloom = 0.0f;
           
    for (int y = -12; y < 13; y++)
    {
        for (int x = -3; x < 4; x++)
        {
            float2 texcoord = In.TexCoord + float2(x * 3.0f / 1280, y * 3.5f / 720.0f);
            float4 color = gTexture.Sample(gSamplerState, texcoord);
            float bright = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
            if (bright > 0.7f && (x != 0 || y != 0))
            {
                bloom += color / (x * x + y * y);
            }
        }

    }

    outDiffuse.rgb += bloom.rgb;
    
    float gray;
    gray = 0.299 * outDiffuse.r + 0.587 * outDiffuse.g + 0.114 * outDiffuse.b;
    clip(gray - 1.0f);
    
}