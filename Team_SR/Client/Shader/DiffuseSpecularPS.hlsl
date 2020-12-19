#define MAX_LIGHT_NUM 16

struct PS_INPUT
{
    float2 UV : TEXCOORD0;
    float3 ViewDirection : TEXCOORD1;
    float3 Normal : TEXCOORD2;
    float3 Tangent : TEXCOORD3;
    float3 BiNormal : TEXCOORD4;
    float3 WorldLocation : TEXCOORD5;
};

float4 LightLocation[MAX_LIGHT_NUM];
float4 LightDiffuse[MAX_LIGHT_NUM];
float LightRadius[MAX_LIGHT_NUM];

sampler2D DiffuseSampler;
sampler2D SpecularSampler;
sampler2D NormalSampler;

float4 GlobalAmbient;

int LightNum;
float Shine;

float4 main(PS_INPUT Input) : COLOR
{
    Input.Normal = normalize(Input.Normal);
    Input.Tangent = normalize(Input.Tangent);
    Input.BiNormal = normalize(Input.BiNormal);
    Input.ViewDirection = normalize(Input.ViewDirection);
    
    float4 DiffuseTexColor = tex2D(DiffuseSampler, Input.UV);
    float4 SpecularTexColor = tex2D(SpecularSampler, Input.UV);
    
    float3 OutputColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < LightNum; ++i)
    {
        // WorldLightDirection Calc
        float3 LightDirection = Input.WorldLocation - LightLocation[i].xyz;
        float Distance = length(LightDirection);
        LightDirection = normalize(LightDirection);
    
	// Diffuse Color Calc in World Coord System....
        float3 DiffuseDot= dot(-LightDirection, Input.Normal);
	// Reflection Vector Calc in World Coord System.... For Specular Color Calc
        float3 ReflectionVector = reflect(LightDirection, Input.WorldLocation);
        
        float3 Diffuse = LightDiffuse[i].rgb * DiffuseTexColor.rgb * saturate(DiffuseDot);
       
        float3 Specular = 0;
        
        if (Diffuse.x > 0)
        {
            Specular = saturate(dot(ReflectionVector, -Input.ViewDirection));
            Specular = pow(Specular, Shine);
            Specular = Specular * SpecularTexColor.rgb * LightDiffuse[i].rgb;
        }
	
        float3 CurrentAmbient = GlobalAmbient.rgb * DiffuseTexColor.xyz;
	
        float4 CurrentColor = float4((CurrentAmbient + Diffuse + Specular).xyz, 0.0f);
        
        float factor = 1.f - (Distance / LightRadius[i]);
        factor = saturate(factor);
	
        CurrentColor.rgb *= factor;
        
        OutputColor += CurrentColor;
    }
    
    return float4(OutputColor.xyz, DiffuseTexColor.a);
};