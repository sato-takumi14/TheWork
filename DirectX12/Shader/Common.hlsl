
cbuffer MATRIX : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct MaterialData
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float Dummy[3]; //16byte‹«ŠE—p
};

struct InstanceData
{
    matrix World;
    MaterialData Material;
};

StructuredBuffer<InstanceData> InstanceInfo : register(t3);
