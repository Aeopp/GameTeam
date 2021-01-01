matrix World;
matrix Projection;

struct VS_INPUT
{
    float4 Location : POSITION;
};

struct VS_OUTPUT
{
    float4 Location : POSITION;
    float2 ScreenPos : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;

    Output.Location = mul(Input.Location,World);
    Output.ScreenPos = Output.Location.xy;
    Output.Location = mul(Output.Location, Projection);
    
    return Output;
};