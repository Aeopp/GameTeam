struct PS_INPUT
{
    float2 UV : TEXCOORD0;
};

sampler2D DiffuseSampler;

float4 main(PS_INPUT Input) : COLOR
{
    float4 Color = tex2D(DiffuseSampler, Input.UV);
    
    return Color;
};
    