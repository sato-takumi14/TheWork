
#include"Common.hlsl"

static const float pi = 3.1415926525f;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    
    outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float2 uv = In.TexCoord - 0.5f;
    
    float angle = atan2(uv.x, uv.y) + pi;
    
    //dif       �x�[�X�J���[
    //amb       ���X�g�J���[
    //color     �����Ă��镔��
    //hp.x      ���݂�HP
    //hp.y      �ő��HP
    //hp.z      �O���HP
    
    float gage = frac(angle * 0.5f / pi) * hp.y;
    outDiffuse.rgb += amb.rgb * step(hp.z, gage);
    outDiffuse.rgb += dif.rgb * (1.0f - step(hp.x, gage));
    outDiffuse.rgb += color.rgb * (1.0f - step(hp.z, gage)) * step(hp.x, gage);
    float dist = length(uv) / 0.5f;
    outDiffuse.a = (1.0f - step(1.0f, dist)) * step(0.3f, dist);
    
}