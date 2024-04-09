
struct PS_IN
{
    float4 Position     : SV_POSITION;
    float4 Normal       : NORMAL0;
    float4 Diffuse      : COLOR0;
    float2 TexCoord     : TEXCOORD0;
};

SamplerState gSampler : register(s0);
Texture2D gTex : register(t0);

float4 main(in PS_IN input) : SV_Target
{
    return gTex.Sample(gSampler, input.TexCoord);
}
