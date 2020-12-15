struct PS_INPUT
{
	float2 UV : TEXCOORD0;
	float3 Diffuse : TEXCOORD1;
	float3 ViewDirection : TEXCOORD2;
	float3 Reflection : TEXCOORD3;
	float3 Distance : TEXCOORD4;
    float3 Normal : TEXCOORD5;
    float3 Tangent : TEXCOORD6;
    float3 BiNormal : TEXCOORD7;
};

sampler2D DiffuseSampler;
sampler2D SpecularSampler;
sampler2D NormalSampler;

float4 GlobalAmbient;

float Shine;
float Range;
float4 LightColor;
float4 Specular;
float4 Emissive;
float4 Ambient;

float4 main(PS_INPUT Input) : COLOR
{
    Input.Normal = normalize(Input.Normal);
    Input.Tangent = normalize(Input.Tangent);
    Input.BiNormal = normalize(Input.BiNormal);
	
	float4 DiffuseTexColor = tex2D(DiffuseSampler, Input.UV);
	float3 Diffuse = LightColor.xyz * DiffuseTexColor.rgb * saturate(Input.Diffuse);
	
	// Renormalize.....
	float3 Reflection = normalize(Input.Reflection);
	float3 ViewDirection = normalize(Input.ViewDirection);
	float3 Specular = 0;
	
	if (Input.Diffuse.x > 0)
	{
		Specular = saturate(dot(Reflection, -ViewDirection));
        Specular = pow(Specular, Shine);
		
		float4 SpecularIntensity = tex2D(SpecularSampler, Input.UV);
        Specular *= SpecularIntensity.rgb * LightColor.xyz;
    };
	
    float3 CurrentAmbient = Ambient.xyz * DiffuseTexColor.xyz;
	
    float4 OutputColor = float4(CurrentAmbient + Diffuse + Specular, DiffuseTexColor.a);
    float factor = 1.f - (Input.Distance / Range);
	factor =saturate(factor);
	
	OutputColor.rgb *= factor;
	return OutputColor;
};