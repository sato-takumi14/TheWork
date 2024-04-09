
#include"Common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    float4 normal = normalize(In.Normal);
    float light = -dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);
    
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    float3 lightDir = Light.Position.xyz - In.WorldPosition.xyz;
    float len = length(lightDir);
    lightDir /= len;
    
    float intensity = saturate(dot(normalize(outDiffuse.rgb), -lightDir));
    
    float4 lightResult = intensity * color;
    
    outDiffuse += lightResult;
    
}