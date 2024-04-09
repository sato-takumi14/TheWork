
#include"Common.hlsl"

Texture2D gTex : register(t0);
Texture2D gRenderingTex : register(t1);
SamplerState gSample : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    outDiffuse = gRenderingTex.Sample(gSample, In.TexCoord);
    outDiffuse.a = In.Diffuse.a;

}