
#include"Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
	
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
	
    Out.Normal = worldNormal;
	
    Out.Diffuse = In.Diffuse;

    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;
    
    Out.Binormal = In.Binormal;
    Out.Tangent = In.Tangent;

    Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
}