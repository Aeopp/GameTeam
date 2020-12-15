struct PS_INPUT
{
	float2 UV : TEXCOORD0;
	float3 Diffuse : TEXCOORD1;
	float3 ViewDirection : TEXCOORD2;
	float3 Reflection : TEXCOORD3;
	float3 Distance : TEXCOORD4;
};

sampler2D DiffuseSampler;
sampler2D SpecularSampler;

float3 LightColor;

float4 main(PS_INPUT Input) : COLOR
{
	float4 DiffuseTexColor = tex2D(DiffuseSampler, Input.UV);
	float3 Diffuse = LightColor * DiffuseTexColor.rgb * saturate(Input.Diffuse);
	
	// Renormalize.....
	float3 Reflection = normalize(Input.Reflection);
	float3 ViewDirection = normalize(Input.ViewDirection);
	float3 Specular = 0;
	
	if (Input.Diffuse.x > 0)
	{
		Specular = saturate(dot(Reflection, -ViewDirection));
		Specular = pow(Specular, 20.0f);
		
		float4 SpecularIntensity = tex2D(SpecularSampler, Input.UV);
		Specular *= SpecularIntensity.rgb * LightColor;
	};
	
	float3 Ambient = float3(0.1f, 0.1f, 0.1f) * DiffuseTexColor.xyz;
	
	float4 OutputColor = float4(Ambient + Diffuse + Specular, DiffuseTexColor.a);
	float factor = 1.f - (Input.Distance / 30.f);
	factor =saturate(factor);
	
	OutputColor.rgb *= factor;
	return OutputColor;
};