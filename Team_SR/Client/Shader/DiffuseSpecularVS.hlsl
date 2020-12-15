matrix World;
matrix View;
matrix Projection;

float4 WorldLightLocation;
float4 WorldCameraLocation;

struct VS_INPUT
{
	float4 Location : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Location : POSITION;
	float2 UV : TEXCOORD0;
	float3 Diffuse : TEXCOORD1;
	float3 ViewDirection: TEXCOORD2;
	float3 Reflection : TEXCOORD3;
	float3 Distance : TEXCOORD4;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;
	
	// Vertex Location : Local Coord -> World Coord 
	Output.Location = mul(Input.Location, World);
	
	// WorldLightDirection Calc
	float3 LightDirection = Output.Location.xyz - WorldLightLocation.xyz;
	Output.Distance = length(LightDirection);
	LightDirection = normalize(LightDirection);
	
	// WorldViewDirection Calc
	float3 ViewDirection = normalize(Output.Location.xyz - WorldCameraLocation.xyz);
	Output.ViewDirection = ViewDirection;
	
	// Vertex Location : World Coord -> View Coord ... -> Projection Coord
	Output.Location = mul(Output.Location, View);
	Output.Location = mul(Output.Location, Projection);
	
	// Vertex Normal -> Local Coord -> World Coord
	float3 WorldNormal = mul(Input.Normal, (float3x3) World);
	WorldNormal = normalize(WorldNormal);
	
	// Diffuse Color Calc in World Coord System....
	Output.Diffuse = dot(-LightDirection, WorldNormal);
	// Reflection Vector Calc in World Coord System.... For Specular Color Calc
	Output.Reflection = reflect(LightDirection, WorldNormal);
	
	Output.UV = Input.UV;
	
	return Output;
};
