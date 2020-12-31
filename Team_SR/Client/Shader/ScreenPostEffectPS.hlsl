struct PS_INPUT
{
    float2 UV : TEXCOORD0;
};

sampler2D DiffuseSampler;
float AlphaCoefft;
int Flag;

float4 main(PS_INPUT Input) : COLOR
{
    float4 Color = tex2D(DiffuseSampler, Input.UV);
    
    if (Flag == 1)
    {
        Color.rgb =     float3(71.f / 255.f,
                                 108.f / 255.f,
                                   255.f / 255.f);
    }
    
    Color.a *= AlphaCoefft;
  
    return Color;
};
    