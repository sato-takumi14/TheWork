
#include"Common.hlsl"

Texture2D gTexture : register(t0);
Texture2D gShadow : register(t1);
SamplerState gState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    outDiffuse = gTexture.Sample(gState, In.TexCoord);
    outDiffuse *= In.Diffuse;
    
    In.ShadowPosition.xyz /= In.ShadowPosition.w;
    In.ShadowPosition.x = In.ShadowPosition * 0.5f + 0.5f;
    In.ShadowPosition.y = -In.ShadowPosition.y * 0.5f + 0.5f;
    
    float depth = gShadow.Sample(gState, In.TexCoord).r;
    float range = 0.1f;
    const int count = 6;
    float shadow = 0.0f;
    
    for (int y = -count / 2; y < count / 2; y++)
    {
        
        for (int x = -count / 2; x < count / 2; x++)
        {
            
            float offset = float2(y, x) / (count * range);
            float uv = gShadow.Sample(gState, In.ShadowPosition.xy + offset).r;
            shadow += (depth <= uv) ? 1.0f : 0.0f;
            
        }
        
    }
    
    shadow /= (count * count);
    outDiffuse.rgb *= shadow;
    
}