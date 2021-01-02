struct PS_INPUT
{
    float2 ScreenPos : TEXCOORD0;
};

float2 PlayerScreenLocation;
float DistanceMin;
int Relative;

float4 main(PS_INPUT Input) : COLOR
{
    //if (DistanceMin < distance(Input.ScreenPos.x, PlayerScreenLocation.x))
    //{
    //    discard;
    //}
    
    //if (DistanceMin < distance(Input.ScreenPos.y, PlayerScreenLocation.y))
    //{
    //    discard;
    //}
    if (Relative == 1)
    {
        float CurrentDistance = distance(Input.ScreenPos, PlayerScreenLocation);
        if (DistanceMin < CurrentDistance)
        {
            discard;
        }
    
        return float4(1.0, 1.0, 1.0, 1.0f - (CurrentDistance / DistanceMin));
    }
  
    return float4(0.5, 0.5, 0.5, 1.0f);
};
    