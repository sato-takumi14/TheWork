

#include "Common.hlsl"


Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 normal = normalize(In.Normal);
    float light = -dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);
		
    outDiffuse = gTexture.Sample(gSamplerState, In.TexCoord);
    outDiffuse.rgb *= light * In.Diffuse.rgb;
    outDiffuse.a *= In.Diffuse.a;
    
    float bloom = 0.0f;
    
    for (int y = -12; y < 13; y++)
    {
        for (int x = -3; x < 4; x++)
        {
            float2 texcoord = In.TexCoord + float2(x * 3.0f / 1280, y * 3.5f / 720.0f);
            float4 color = gTexture.Sample(gSamplerState, texcoord);
            float bright = color.r * 0.299f + color.g * 0.587 + color.b * 0.114f;
            if (bright > 0.7f && (x != 0 || y != 0))
            {
                bloom += color / (x * x + y * y);
            }
        }

    }

    outDiffuse.rgb += bloom;
    
}

