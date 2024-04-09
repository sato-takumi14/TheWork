cbuffer WorldBuffer : register(b0)
{
    matrix World;
}

cbuffer ViewBuffer : register(b1)
{
    matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

struct VS_IN
{
    float4 Position     : POSITION0;
    float4 Normal       : NORMAL0;
    float4 Tangent      : TANGENT0;
    float4 Binormal     : BINORMAL0;
    float4 Diffuse      : COLOR0;
    float2 TexCoord     : TEXCOORD0;
    uint   InstanceId   : SV_InstanceID;
    //matrix WorldMatrix;
};

struct PS_IN
{
    float4 Position         : SV_POSITION;
    float4 WorldPosition    : POSITION0;
    float4 Normal           : NORMAL0;
    float4 Tangent          : TANGENT0;
    float4 Binormal         : BINORMAL0;
    float4 Diffuse          : COLOR0;
    float2 TexCoord         : TEXCOORD0;
	
    float4 ShadowPosition   : POSITION1; //ライトカメラから見たピクセルの座標	
};

struct LIGHT
{
    bool Enable;
    bool3 Dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
	
    matrix View; //ライトカメラのビュー行列
    matrix Projection; //ライトカメラのプロジェクション行列
    
    float4 Position;
	
};

struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float Dummy[3]; //16byte境界用
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

cbuffer LightBuffer : register(b4)
{
    LIGHT Light;
}

cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition;
}


cbuffer ParamBuffer : register(b6)
{
    float4 hp;
    float4 dif;
    float4 amb;
    float4 color;
}

struct WIPE
{
    float range;
    float threshold;
    float dummy[2];
};

struct DISSOLVE
{
    float range;
    float threshold;
    float dummy[2];
    float4 color;
};

struct InstanceData
{
    matrix WorldMatrix;
    MATERIAL Material;
    bool Active;
};

cbuffer WIPEBuffer : register(b7)
{
    WIPE Wipe;
}

cbuffer DISSOVEBuffer : register(b8)
{
    DISSOLVE Dissolve;
}

StructuredBuffer<InstanceData> InstanceInfo : register(t3);