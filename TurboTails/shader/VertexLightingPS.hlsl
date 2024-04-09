

#include "Common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
		
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);		    
	//�����x�͋P�x�ɉe�����Ȃ��̂ł��̂܂܎g��
    outDiffuse.a *= In.Diffuse.a;
    
    //�@���𐳋K�����Ċi�[����
    float4 normal = normalize(In.Normal);

	//�����o�[�g
    float light = -dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);
   
	//�u�����t�H�����ʔ���
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    float3 halfv = eyev + Light.Direction.xyz;
    halfv = normalize(halfv);

    float specular = -dot(halfv, normal.xyz);
    saturate(specular);
    specular = pow(specular, 30);
    outDiffuse.rgb = saturate(outDiffuse.rgb + specular);
    
}

