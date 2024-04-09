
#include "Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    
    matrix wvp;
    matrix world;
    
    world = InstanceInfo[In.InstanceId].WorldMatrix;
    
    wvp = mul(world, View);
    wvp = mul(wvp, Projection);
	
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, world);
    worldNormal = normalize(worldNormal);
	
    Out.Normal = worldNormal;
	
    Out.Diffuse = In.Diffuse;
    
    Out.Position = mul(In.Position, wvp);
    Out.WorldPosition = mul(In.Position, world);
    Out.TexCoord = In.TexCoord;
    
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.0f), World));
    Out.Binormal = In.Binormal;
    Out.Tangent = In.Tangent;

}

