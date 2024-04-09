
#include "Common.hlsl"


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
	
    Out.Diffuse = Material.Diffuse;

    Out.Position = mul(In.Position, wvp);
    Out.WorldPosition = mul(In.Position, World);
    Out.TexCoord = In.TexCoord;

    Out.ShadowPosition = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Out.Tangent = In.Tangent;
    Out.Binormal = In.Binormal;
    
}

