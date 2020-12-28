struct PS_INPUT
{
    float2 ScreenPos : TEXCOORD0;
};

float2 PlayerScreenLocation;
float DistanceMin;


float4 main(PS_INPUT Input) : COLOR
{
    if (DistanceMin < distance(Input.ScreenPos.x, PlayerScreenLocation.x))
    {
        discard;
    }
    
    if (DistanceMin < distance(Input.ScreenPos.y, PlayerScreenLocation.y))
    {
        discard;
    }
    
    return float4(0.5, 0.5, 0.5, 1);
};
    