// Used for clearing the G-Buffer
struct GBUFFERCLEARVSI
{
	float3 Position	:	POSITION0;
};

struct GBUFFERCLEARVSO
{
	float4 Position	:	POSITION0;
};

GBUFFERCLEARVSO ClearMainVS( GBUFFERCLEARVSI p_Input )
{
	GBUFFERCLEARVSO Output;
	Output.Position = float4( p_Input.Position, 1 );
	return Output;
}

struct GBUFFERCLEARPSO
{
	float4 Colour	:	COLOR0;
	float4 Normal	:	COLOR1;
	float4 Depth	:	COLOR2;
};

GBUFFERCLEARPSO ClearMainPS( GBUFFERCLEARVSO p_Input )
{
	GBUFFERCLEARPSO Output;

	// GOOM for colour
	Output.Colour.rb = 0.13f;
	Output.Colour.ga = 0.0f;

	// For the normal, gray works well
	Output.Normal.rgb = 0.5f;

	// Nothing in Normal.a, yet
	Output.Normal.a = 0.0f;

	// Depth is at maximum depth
	Output.Depth = 1.0f;

	return Output;
}

technique ClearGBuffer
{
	pass Pass0
	{
		VertexShader = compile vs_3_0 ClearMainVS( );
		PixelShader = compile vs_3_0 ClearMainPS( );
	}
}