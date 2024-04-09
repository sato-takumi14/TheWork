
#include"Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse;
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.0f), World));
    Out.Binormal = In.Binormal;
    Out.Tangent = In.Tangent;
    
}