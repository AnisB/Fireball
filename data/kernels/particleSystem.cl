#ifndef M_PI 
#define M_PI 3.14159265359
#endif
// RNG STUFF
ulong MWC_AddMod64(ulong a, ulong b, ulong M);
ulong MWC_MulMod64(ulong a, ulong b, ulong M);
ulong MWC_PowMod64(ulong a, ulong e, ulong M);
uint2 MWC_SkipImpl_Mod64(uint2 curr, ulong A, ulong M, ulong distance);
uint2 MWC_SeedImpl_Mod64(ulong A, ulong M, uint vecSize, uint vecOffset, ulong streamBase, ulong streamGap);


typedef struct{ uint x; uint c; } mwc64x_state_t;

void MWC64X_Step(mwc64x_state_t *s);
void MWC64X_Skip(mwc64x_state_t *s, ulong distance);
void MWC64X_SeedStreams(mwc64x_state_t *s, ulong baseOffset, ulong perStreamOffset);
float MWC64X_NextUint(mwc64x_state_t *s);

float3 getRandomDirection(mwc64x_state_t* rng);



ulong MWC_AddMod64(ulong a, ulong b, ulong M)
{
	ulong v=a+b;
	if( (v>=M) || (v<a) )
		v=v-M;
	return v;
}
ulong MWC_MulMod64(ulong a, ulong b, ulong M)
{	
	ulong r=0;
	while(a!=0){
		if(a&1)
			r=MWC_AddMod64(r,b,M);
		b=MWC_AddMod64(b,b,M);
		a=a>>1;
	}
	return r;
}
ulong MWC_PowMod64(ulong a, ulong e, ulong M)
{
	ulong sqr=a, acc=1;
	while(e!=0){
		if(e&1)
			acc=MWC_MulMod64(acc,sqr,M);
		sqr=MWC_MulMod64(sqr,sqr,M);
		e=e>>1;
	}
	return acc;
}


uint2 MWC_SkipImpl_Mod64(uint2 curr, ulong A, ulong M, ulong distance)
{
	ulong m=MWC_PowMod64(A, distance, M);
	ulong x=curr.x*(ulong)A+curr.y;
	x=MWC_MulMod64(x, m, M);
	return (uint2)((uint)(x/A), (uint)(x%A));
}

uint2 MWC_SeedImpl_Mod64(ulong A, ulong M, uint vecSize, uint vecOffset, ulong streamBase, ulong streamGap)
{
	// This is an arbitrary constant for starting LCG jumping from. I didn't
	// want to start from 1, as then you end up with the two or three first values
	// being a bit poor in ones - once you've decided that, one constant is as
	// good as any another. There is no deep mathematical reason for it, I just
	// generated a random number.
	enum{ MWC_BASEID = 4077358422479273989UL };
	
	ulong dist=streamBase + (get_global_id(0)*vecSize+vecOffset)*streamGap;
	ulong m=MWC_PowMod64(A, dist, M);
	
	ulong x=MWC_MulMod64(MWC_BASEID, m, M);
	return (uint2)((uint)(x/A), (uint)(x%A));
}


enum{ MWC64X_A = 4294883355U };
enum{ MWC64X_M = 18446383549859758079UL };

void MWC64X_Step(mwc64x_state_t *s)
{
	uint X=s->x, C=s->c;
	
	uint Xn=MWC64X_A*X+C;
	uint carry=(uint)(Xn<C);				// The (Xn<C) will be zero or one for scalar
	uint Cn=mad_hi(MWC64X_A,X,carry);  
	
	s->x=Xn;
	s->c=Cn;
}


void MWC64X_Skip(mwc64x_state_t *s, ulong distance)
{
	uint2 tmp=MWC_SkipImpl_Mod64((uint2)(s->x,s->c), MWC64X_A, MWC64X_M, distance);
	s->x=tmp.x;
	s->c=tmp.y;
}

void MWC64X_SeedStreams(mwc64x_state_t *s, ulong baseOffset, ulong perStreamOffset)
{
	uint2 tmp=MWC_SeedImpl_Mod64(MWC64X_A, MWC64X_M, 1, 0, baseOffset, perStreamOffset);
	s->x=tmp.x;
	s->c=tmp.y;
}
float MWC64X_NextUint(mwc64x_state_t *s)
{
	float res=s->x ^ s->c;
	MWC64X_Step(s);
	return res/0xFFFFFFFF;
}

float3 getRandomDirection(mwc64x_state_t* rng)
{
	float theta = MWC64X_NextUint(rng)*2.0*M_PI;
	float phi = acos(2.0*MWC64X_NextUint(rng)-1.0);
	return float3(cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi));
}

// Init step
__kernel void initParticles(__global float3* pos, 
							__global float3* vec, 
							__global float3* color, 
							__global float* lifetime, 
							const float3 position, 
							const float3 meanColor, 
							const float3 colorVariation, 
							const float meanDuration, 
							const float durationVariance, 
							uint nbParticles)                                           
{                                                                      
	int i = get_global_id(0); 
	int j = get_global_size (0);

	uint size = nbParticles/j;

	uint boundUp = size*(i+1);
	uint boundLow = size*(i);

	mwc64x_state_t rng;
	MWC64X_SeedStreams(&rng, boundLow, 2*size);
	for(uint particle = boundLow; particle < boundUp; ++particle )
	{
		pos[particle] = position;

		vec[particle] = getRandomDirection(&rng);

		float var = (MWC64X_NextUint(&rng)-0.5f)*2.0f;
		color[particle] = meanColor + colorVariation*var;

		float var2 = (MWC64X_NextUint(&rng)-0.5f)*2.0f;
		lifetime[particle] = meanDuration + durationVariance*var2;
	}
}



// Init step
__kernel void update(__global float3* pos, 
						__global float3* vec, 
						__global float* lifetime, 
						
						const float3 position, 
						const float3 meanColor, 
						const float3 colorVariation, 
						const float meanDuration, 
						const float durationVariance, 
						uint nbParticles,
						float parTime)                                           
{                                                                      
	int i = get_global_id(0); 
	int j = get_global_size (0);

	uint size = nbParticles/j;

	uint boundUp = size*(i+1);
	uint boundLow = size*(i);

	mwc64x_state_t rng;
	MWC64X_SeedStreams(&rng, boundLow, 2*size);

	for(uint particle = boundLow; particle < boundUp; ++particle )
	{
		pos[particle] = position + vec[particle]*parTime;

		lifetime[particle] = lifetime[particle] - parTime;
		if( lifetime[particle] < 0.0)
		{
			pos[particle] = position;
			float var2 = (MWC64X_NextUint(&rng)-0.5f)*2.0f;
			lifetime[particle] = meanDuration + durationVariance*var2;

			vec[particle] = getRandomDirection(&rng);
		}
	}
}