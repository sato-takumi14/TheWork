

#include "Common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse.rgb = float3(0.0, 0.0, 0.0);
    
    float dissolveValue = g_Texture.Sample(g_SamplerState, In.TexCoord);
    float threshold = Wipe.threshold * (1.0 + Wipe.range) - Wipe.range;
    float rate = saturate((dissolveValue - threshold) / Wipe.range);
    
    outDiffuse.a = rate;
}

