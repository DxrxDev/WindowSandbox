cbuffer cBuf{
	float4 colours[6];
}

float4 main(uint tID : SV_PRIMITIVEID) : SV_Target{
	return colours[tID/2];
}