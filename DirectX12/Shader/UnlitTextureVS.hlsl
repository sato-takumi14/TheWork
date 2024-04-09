
cbuffer MATRIX : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_IN
{
    float3 Position     : POSITION0;
    float3 Normal       : NORMAL0;
    float4 Diffuse      : COLOR0;
    float3 Tangent      : TANGENT;
    float2 TexCoord     : TEXCOORD0;
};

struct VS_OUT
{
    float4 Position     : SV_POSITION;
    float4 Normal       : NORMAL0;
    float4 Diffuse      : COLOR0;
    float2 TexCoord     : TEXCOORD00;    
};

VS_OUT main(in VS_IN input)
{
    
    VS_OUT output;
    
    matrix wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    float4 pos = mul(World, float4(input.Position, 1.0f));
    pos = mul(View, pos);
    pos = mul(Projection, pos);
    
    output.Position = pos;
    output.Diffuse = input.Diffuse;
    output.Normal = normalize(float4(input.Normal, 0.0f));
    output.TexCoord = input.TexCoord;
    
    return output;
    
}
