struct InputStruct {
    float2 inTexCoord : texCoord;
    float3 inPos : Position;
};

struct ReturnStruct {
    float2 outTexCoord : TexCoord;
    float4 outPos : SV_POSITION;
};

cbuffer CBuff {
    matrix trans;
};

ReturnStruct main(InputStruct input){
    ReturnStruct RS;
    RS.outTexCoord = input.inTexCoord;
    RS.outPos = mul(float4(input.inPos, 1.0f), trans);
    return RS;
}