matrix World;
matrix Projection;

struct VS_INPUT
{
    float4 Location : POSITION;
};

struct VS_OUTPUT
{
    float4 Location : POSITION;

};

VS_OUTPUT main(VS_INPUT Input)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;

    Output.Location = mul(Input.Location,World);
    Output.Location = mul(Output.Location,Projection);
    
    
    return Output;
};