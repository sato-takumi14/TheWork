

#include "Common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_Wipe : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //法線を正規化して格納する
    float4 normal = normalize(In.Normal);
    
    //ランバート
    float light = -dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);
    
    //テクスチャから色をサンプリングする
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse.rgb *= light;
    outDiffuse.a *= In.Diffuse.a;
    
    //ブリンフォン鏡面反射
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    float3 halfv = eyev + Light.Direction.xyz;
    halfv = normalize(halfv);
    
    float specular = -dot(halfv, normal.xyz);
    saturate(specular);
    specular = pow(specular, 30);
    outDiffuse.rgb = saturate(outDiffuse.rgb + specular);
    
    //ディゾルブ処理
    float dissolveVale = g_Wipe.Sample(g_SamplerState, In.TexCoord);
    float threshold = Dissolve.threshold * (1.0 + Dissolve.range) - Dissolve.range;
    float rate = saturate((dissolveVale - threshold) / Dissolve.range);
    outDiffuse.a = rate;
    
    outDiffuse.rgb = lerp(outDiffuse.rgb, Dissolve.color.rgb, 1 - pow(rate, 5));
    
}

