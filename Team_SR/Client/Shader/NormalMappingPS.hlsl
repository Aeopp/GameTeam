sampler2D DiffuseSampler;
sampler2D SpecularSampler;
sampler2D NormalSampler;
float4 LightColor;

struct PS_INPUT
{
	float2 UV : TEXCOORD0;
	float3 LightDir : TEXCOORD1;
	float3 ViewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

float4 main(PS_INPUT Input)  :COLOR
{
	float3 TangentNormal = tex2D(NormalSampler, Input.UV).xyz;
	TangentNormal = normalize(TangentNormal * 2 - 1);
	
	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
	TBN = transpose(TBN);
	float3 WorldNormal = mul(TBN, TangentNormal);
	
	float4 albedo = tex2D(DiffuseSampler, Input.UV);
	float3 lightDir = normalize(Input.LightDir);
	float3 diffuse = saturate(dot(WorldNormal, -lightDir));
	diffuse = LightColor.xyz * albedo.rgb * diffuse;
	
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, WorldNormal);
		float3 viewDir = normalize(Input.ViewDir);
		
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);
		
		float4 specularIntensity = tex2D(SpecularSampler, Input.UV);
		specular *= specularIntensity.rgb * LightColor.xyz;
	};
	
	float3 ambient = float3(0.05f, 0.05f, 0.05f) * albedo.xyz;
	
	return float4(ambient + diffuse + specular, 1);
}