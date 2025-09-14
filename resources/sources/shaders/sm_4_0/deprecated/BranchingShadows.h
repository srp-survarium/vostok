#define NUM_REFINING_SAMPLE_CHUNKS 6
#define NUM_EDGE_SAMPLE_CHUNKS 4

static const float4 EdgeSampleOffsets[NUM_EDGE_SAMPLE_CHUNKS] = 
{
    float4(-0.071794f, -0.767445f, 0.419721f, -0.704903f),
	float4(-0.000848f, -0.000485f, -0.592816f, -0.303198f),
    float4(0.841066f, -0.308718f, 0.228044f, 0.848170f),
	float4(0.612645f, 0.200965f, -0.588486f, 0.690574f)
};

static const float4 RefiningSampleOffsets[NUM_REFINING_SAMPLE_CHUNKS * 2] = 
{
    float4(-0.840328f, 0.541797f, 0.662232f, -0.656196f),
	float4(0.559291f, 0.435458f, -0.025865f, -0.064991f),
    float4(0.835040f, 0.210535f, 0.271059f, -0.505058f),
	float4(0.248493f, 0.767084f, -0.596259f, 0.621869f),
    float4(-0.326291f, 0.175500f, 0.643448f, -0.439298f),
	float4(-0.406031f, -0.841996f, -0.795511f, -0.105329f),
    float4(0.309025f, 0.352076f, -0.162909f, 0.375198f),
	float4(-0.726905f, -0.446674f, 0.027331f, -0.337222f),
    float4(0.543546f, 0.735876f, 0.309095f, -0.710946f),
	float4(-0.582790f, 0.154986f, -0.203460f, -0.272108f),
    float4(0.491919f, -0.171667f, -0.405075f, -0.447730f),
	float4(-0.416467f, -0.072390f, -0.025764f, 0.565929f)
};

float4x4 ScreenToShadowMatrix;

static const float2 ShadowBufferSize = float2(SMAP_size, SMAP_size);

float EdgeSamplesManualPCF(float4 ShadowPosition, float2 JitterTC)
{
	float ShadowCoverage = 0;

	float2 RandomCosSin = tex2D(jitter0, JitterTC).xy * 2 - 1;

	RandomCosSin = normalize(RandomCosSin);
	
	//used to normalize accumulated coverage, since there are 2 samples per chunk
	float SampleWeight = .5 / NUM_EDGE_SAMPLE_CHUNKS;
	float2 FlipVector = float2(1, -1);
	
	//Go through each edge-detecting chunk and take samples
	[unroll]
	for(int ChunkIndex = 0;ChunkIndex < NUM_EDGE_SAMPLE_CHUNKS;ChunkIndex++)
	{
		float2	ShadowDepths;
		float4	ChunkSampleOffsets;	
		float4	TexCoords;
		float2	Mask;
		
		//Rotate the edge sample offsets, using the cos and sin of a random angle previously looked up,
		//and add this offset to the base.  Two samples are rotated together to take advantage of vector operations.
		//For reference, this is how a single sample is rotated:
		//rotatedOffset.x= cos(theta) * sampleOffset.x + sin(theta) * sampleOffset.y;
		//rotatedOffset.y= -sin(theta) * sampleOffset.x + cos(theta) * sampleOffset.y;
		ChunkSampleOffsets = EdgeSampleOffsets[ChunkIndex].xyzw / SMAP_size.xxxx;
		TexCoords = ShadowPosition.xyxy + FlipVector.xyxy * RandomCosSin.xyxy * ChunkSampleOffsets.xxzz + RandomCosSin.yxyx * ChunkSampleOffsets.yyww;

		//take two samples of the depth buffer
		ShadowDepths.x = tex2D(s_smap,TexCoords.xy).r;
		ShadowDepths.y = tex2D(s_smap,TexCoords.zw).r;
		
		//depth compare and combine the results, then accumulate with previous results
		Mask = ShadowPosition.z < ShadowDepths ? float2(1,1) : float2(0,0);
		ShadowCoverage = ShadowCoverage + dot(Mask, SampleWeight.xx);
		
	}
	return ShadowCoverage;
}

float RefiningSamplesManualPCF(float4 ShadowPosition)
{
	//used to normalize accumulated coverage, since there are 4 samples per chunk
	float SampleWeight = .25 / NUM_REFINING_SAMPLE_CHUNKS;
	float RefinedShadowCoverage = 0;
	
	//Go through each refining chunk and take samples
	[unroll]
	for(int ChunkIndex = 0;ChunkIndex < NUM_REFINING_SAMPLE_CHUNKS;ChunkIndex++)
	{
		float4	ShadowDepths;
		float4	FinalCoords;
		float4	Mask;
		
		FinalCoords = ShadowPosition.xyxy + RefiningSampleOffsets[ChunkIndex * 2].xyzw  / SMAP_size.xxxx;
		
		//use the special tex instruction that is allowed inside a dynamic branch
		//the texture LOD to use is specified in w
		ShadowDepths.x = tex2Dlod(s_smap, float4(FinalCoords.xy, 0, 0)).r;
		ShadowDepths.y = tex2Dlod(s_smap, float4(FinalCoords.zw, 0, 0)).r;
		
		FinalCoords = ShadowPosition.xyxy + RefiningSampleOffsets[ChunkIndex * 2 + 1].xyzw / SMAP_size.xxxx;
		
		ShadowDepths.z = tex2Dlod(s_smap, float4(FinalCoords.xy, 0, 0)).r;
		ShadowDepths.w = tex2Dlod(s_smap, float4(FinalCoords.zw, 0, 0)).r;
		
		//compare and accumulate all 4 results together
		Mask = ShadowPosition.z < ShadowDepths ? float4(1,1,1,1) : float4(0,0,0,0);
		RefinedShadowCoverage = RefinedShadowCoverage + dot(Mask, SampleWeight.xxxx);
	}
	return RefinedShadowCoverage;
}

float GetBranchingShadowCoverage(float4 ShadowPosition, float2 JitterTC)
{
	ShadowPosition.xyz /= ShadowPosition.w;
	float ShadowCoverage = EdgeSamplesManualPCF(ShadowPosition, JitterTC);
	
	//if we're not totally in shadow or totally out, then we are in the penumbra. 
	//take more samples to refine the coverage.
	if (ShadowCoverage > .0001 && ShadowCoverage < .9999) {
	
		float RefinedShadowCoverage = RefiningSamplesManualPCF(ShadowPosition);
		
		//weight the edge coverage with the refined coverage, which has to be normalized first
		ShadowCoverage = lerp(ShadowCoverage, RefinedShadowCoverage, .7);
	}

	// RETURN_COLOR not needed unless writing to SceneColor;
	// ShadowCoverage raised to the 3rd power helps the penumbra edges blend into the surrounding shadow
	return ShadowCoverage * ShadowCoverage * ShadowCoverage;
}