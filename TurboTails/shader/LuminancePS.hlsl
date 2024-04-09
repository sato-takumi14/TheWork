
#include "Common.hlsl"

Texture2D gTexture : register(t0);
SamplerState gSampleState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    outDiffuse = gTexture.Sample(gSampleState, In.TexCoord);
    outDiffuse *= In.Diffuse;
    
    float gray;
    gray = 0.299 * outDiffuse.r + 0.587 * outDiffuse.g + 0.114 * outDiffuse.b;
    clip(gray - 1.0f);
    
}