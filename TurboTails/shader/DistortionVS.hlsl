
#include"Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    
    matrix wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    Out.Position = mul(In.Position, wvp);
    
    float2 tex = Out.Position.xy;
    tex.x = Out.Position.x / Out.Position.w;
    tex.y = Out.Position.y / Out.Position.w;
    
    tex.x = (tex.x * 0.5f) + 0.5f + 0.02f;
    tex.y = (-tex.y * 0.5f) + 0.5f + 0.02f;
    
    Out.TexCoord = saturate(tex);
    
    Out.Normal = normalize(In.Normal);
    Out.Diffuse = In.Diffuse;
    Out.WorldPosition = mul(In.Position, World);
    Out.Binormal = In.Binormal;
    Out.Tangent = In.Tangent;
    Out.ShadowPosition = 0;
}