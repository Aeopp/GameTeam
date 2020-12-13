float4x4 WorldMatrix;
float4x4 WorldViewProjectionMatrix;
float4 WorldLightLocation;
float4 WorldCameraLocation;

struct VS_INPUT
{
	float4 Location : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 BiNormal : BINORMAL;
	float2 UV : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Location : POSITION;
	float2 UV :			TEXCOORD0;
	float3 LightDir : TEXCOORD1;
	float3 ViewDir :	TEXCOORD2;
	float3 T :TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

VS_OUTPUT main(VS_INPUT Input) 
{
	VS_OUTPUT Output;

	Output.Location = mul(Input.Location, WorldViewProjectionMatrix);
	Output.UV = Input.UV;

	float4 WorldLocation = mul(Input.Location, WorldMatrix);
	float3 LightDir = WorldLocation.xyz - WorldLightLocation.xyz;
	Output.LightDir = normalize(LightDir);

	float3 ViewDir = normalize(WorldLocation.xyz - WorldCameraLocation.xyz);
	Output.ViewDir = ViewDir;
	
	float3 WorldNormal = mul(Input.Normal,(float3x3)WorldMatrix);
	Output.N = normalize(WorldNormal);
	
	float3 WorldTangent = mul(Input.Tangent, (float3x3) WorldMatrix);
	Output.T = normalize(WorldTangent);
	
	float3 WorldBiNormal = mul(Input.BiNormal, (float3x3) WorldMatrix);
	Output.B = normalize(WorldBiNormal);
	
	return Output;
}
