cbuffer cbuff{
	float3 col;
};

struct InputStruct {
	float2 inTexCoord : TexCoord;
	float4 inPos : SV_POSITION;
};

Texture2D tex : Texture : register(t0);
SamplerState samplerState : Sampler: register(s0);

float4 main(InputStruct input) : SV_Target{
	return tex.Sample(samplerState, input.inTexCoord);
}
