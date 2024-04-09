
#include"Common.hlsl"

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    outDiffuse = In.Diffuse;
    
    //法線を正規化して格納する
    float4 normal = normalize(In.Normal);

	//ランバート
    float light = -dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);
   
	//ブリンフォン鏡面反射
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    float3 halfv = eyev + Light.Direction.xyz;
    halfv = normalize(halfv);

    float specular = -dot(halfv, normal.xyz);
    saturate(specular);
    specular = pow(specular, 30);
    outDiffuse.rgb = saturate(outDiffuse.rgb + specular);
    
}