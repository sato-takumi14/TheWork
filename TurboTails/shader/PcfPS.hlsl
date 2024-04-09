
#include "Common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureShadow : register(t1);
SamplerState g_SamplerState : register(s0);

static const float filterSize = 1.0;
static const float shadowMapSize = 1280.0;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;
    
    In.ShadowPosition.xyz /= In.ShadowPosition.w;
    In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5;
    In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;
    
    float depth = g_TextureShadow.Sample(g_SamplerState, In.TexCoord).r;
    float range = 0.1f;
    int count = 6;
    float shadow = 0.0f;
    
    for (int y = -count / 2; y < count / 2; y++)
    {
        
        for (int x = -count / 2; x < count / 2; x++)
        {
            
            float2 offset = float2(y, x) / (count * range);
            float uv = g_TextureShadow.Sample(g_SamplerState, In.ShadowPosition.xy + offset).r;
            
            shadow += (depth <= uv) ? 1.0f : 0.0f;
            
        }

    }
    
    shadow /= (count * count);
    outDiffuse.rgb *= shadow;
    
}
