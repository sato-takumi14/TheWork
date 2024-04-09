
#include"Common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp = mul(InstanceInfo[In.InstanceId].WorldMatrix, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp);
    
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.0f), InstanceInfo[In.InstanceId].WorldMatrix));
    
    float light = saturate(-dot(Light.Diffuse.xyz, Out.Normal.xyz));
    Out.Diffuse = light;
    Out.Diffuse.a = In.Diffuse.a;
    
    Out.Tangent = In.Tangent;
    Out.Binormal = In.Binormal;
    Out.TexCoord = In.TexCoord;
    
    matrix lightwvp = mul(InstanceInfo[In.InstanceId].WorldMatrix, Light.View);
    lightwvp = mul(lightwvp, Light.Projection);
    
    Out.ShadowPosition = mul(In.Position, lightwvp);
    
}