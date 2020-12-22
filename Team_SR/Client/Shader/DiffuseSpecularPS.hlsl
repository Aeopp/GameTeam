#define MAX_LIGHT_NUM 16

struct PS_INPUT
{
    float2 UV : TEXCOORD0;
    float3 ViewDirection : TEXCOORD1;
    float3 Normal : TEXCOORD2;
    float3 Tangent : TEXCOORD3;
    float3 BiNormal : TEXCOORD4;
    float3 WorldLocation : TEXCOORD5;
    float ViewZ : TEXCOORD6;
};

float4 LightLocation[MAX_LIGHT_NUM];
float4 LightDiffuse[MAX_LIGHT_NUM];
float LightRadius[MAX_LIGHT_NUM];

sampler2D DiffuseSampler;
sampler2D SpecularSampler;
sampler2D NormalSampler;
samplerCUBE EnvironmentSampler;

float4 GlobalAmbient;

int bSpecularSamplerBind;
int bNormalSamplerBind;
float AlphaLerp;
int LightNum;

float Shine;
float FogEnd;
float FogStart;
float4 FogColor;

float4 main(PS_INPUT Input) : COLOR
{
    Input.Normal = normalize(Input.Normal);
    Input.Tangent = normalize(Input.Tangent);
    Input.BiNormal = normalize(Input.BiNormal);
  
    
    float3 tangentNormal = tex2D(NormalSampler, Input.UV).xyz;
    tangentNormal = normalize(tangentNormal * 2 - 1);
   
    float3x3 TBN = float3x3(normalize(Input.Tangent), 
                            normalize(Input.BiNormal), 
                            normalize(Input.Normal));
    TBN = transpose(TBN);
    float3 worldNormal = mul(TBN, tangentNormal);
    
    Input.ViewDirection = normalize(Input.ViewDirection);
    float3 Normal = worldNormal;
    if (bNormalSamplerBind==0)
    {
        Normal = Input.Normal;
    }
   
 
    float4 DiffuseTexColor = tex2D(DiffuseSampler, Input.UV);
    float4 SpecularTexColor = tex2D(SpecularSampler, Input.UV);
    
    if (bSpecularSamplerBind == 0)
    {
        SpecularTexColor = DiffuseTexColor;
    }
   
    
    float3 OutputColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < LightNum; ++i)
    {
        // WorldLightDirection Calc
        float3 LightDirection = Input.WorldLocation - LightLocation[i].xyz;
        float Distance = length(LightDirection);
        LightDirection = normalize(LightDirection);
    
	// Diffuse Color Calc in World Coord System....
        float3 DiffuseDot= dot(-LightDirection, Normal);
	// Reflection Vector Calc in World Coord System.... For Specular Color Calc
        float3 ReflectionVector = reflect(LightDirection, Normal);
        
        float3 Diffuse = LightDiffuse[i].rgb * DiffuseTexColor.rgb * saturate(DiffuseDot);
       
        float3 Specular = 0;
        float3 Environment = float3(0, 0, 0);
        if (Diffuse.x > 0)
        {
            Specular = saturate(dot(ReflectionVector, -Input.ViewDirection));
            Specular = pow(Specular, Shine);
            Specular = Specular * SpecularTexColor.rgb * LightDiffuse[i].rgb;
            
            float3 ViewReflect = reflect(Input.ViewDirection, Normal);
            Environment = texCUBE(EnvironmentSampler, ViewReflect).rgb;
        }
	
        float3 CurrentAmbient = GlobalAmbient.rgb * DiffuseTexColor.xyz;
	
        float4 CurrentColor = float4((CurrentAmbient + Diffuse + Specular).xyz, 0.0f);
        
        float factor = 1.f - (Distance / LightRadius[i]);
        factor = saturate(factor);
        
        CurrentColor.rgb += (Environment * 0.99f);
        CurrentColor.rgb *= factor;
        OutputColor += CurrentColor;
    }
    
    float FogFactorLinear = (Input.ViewZ - FogStart) / (FogEnd - FogStart);
    //float Density = 1.0f;
    //float FogFactorExp = 1.0f / exp(Input.ViewZ * Density);
    
    float FogFactor = FogFactorLinear;
    
    OutputColor.rgb = (FogColor.rgb * FogFactor) + (OutputColor.rgb * (1.0f - FogFactor));
    
    float OutAlpha = DiffuseTexColor.a;
    
    OutAlpha *= AlphaLerp;
    
    
    return float4(OutputColor.rgb, OutAlpha);
};