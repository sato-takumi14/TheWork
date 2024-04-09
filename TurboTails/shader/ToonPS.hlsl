
#include "Common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_TextureToon : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    float4 normal = normalize(In.Normal);
    
    float light = -dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);

    light = clamp(light, 0.03, 0.97);
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.a *= In.Diffuse.a;
        
    float2 uv = light;
        
    outDiffuse.rgb *= saturate(g_TextureToon.Sample(g_SamplerState, uv).rgb);
    
}
