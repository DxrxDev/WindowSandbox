struct ReturnStruct {
    float3 c : Colour;
    float4 p : SV_POSITION;
};

ReturnStruct main(float2 pos : Position, float3 colour : Colour){
    ReturnStruct RS;
    RS.c = colour;
    RS.p = float4(pos, 0.0f, 1.0f);
    return RS;
}