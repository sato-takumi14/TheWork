
#include"Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp);
    Out.WorldPosition = mul(In.Position, World);
    Out.Normal = normalize(In.Normal);
    Out.Tangent = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Out.Binormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Out.Diffuse = In.Diffuse;
    Out.TexCoord = In.TexCoord;
}