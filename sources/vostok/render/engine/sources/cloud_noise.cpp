#include "pch.h"

#include "cloud_noise.h"
#include "help_math.h"

namespace vostok {
namespace render {

static float const aperiodic_noise_table[8][8] =
{
	{ 0.552941f, 0.050980f, 0.474510f, 0.733333f, 0.007843f, 0.000000f, 0.811765f, 0.517647f },
	{ 0.364706f, 0.737255f, 0.835294f, 0.000000f, 0.000000f, 0.803922f, 1.000000f, 0.364706f },
	{ 0.850980f, 0.054902f, 1.000000f, 0.000000f, 0.694118f, 1.000000f, 0.960784f, 0.850980f },
	{ 0.776471f, 0.788235f, 0.800000f, 1.000000f, 0.556863f, 0.184314f, 0.862745f, 0.784314f },
	{ 0.831373f, 0.427451f, 0.921569f, 0.364706f, 0.000000f, 0.690196f, 0.356863f, 0.827451f },
	{ 0.031373f, 0.596078f, 0.949020f, 0.901961f, 0.721569f, 1.000000f, 0.835294f, 0.011765f },
	{ 0.984314f, 0.525490f, 0.129412f, 0.725490f, 0.015686f, 1.000000f, 0.000000f, 1.000000f },
	{ 0.552941f, 0.000000f, 0.470588f, 0.741176f, 0.000000f, 0.000000f, 0.811765f, 0.517647f }
};

float cloud_noise::evaluate( float x, float y, u32 num_octaves )
{
	num_octaves = math::max( num_octaves, 1u );

	float result = 0.0f;

	for ( u32 octave_index = 0; octave_index < num_octaves; ++octave_index )
	{
		float const octave_scale = static_cast<float>( math::floor( math::pow( 2.0f, octave_index ) ) );

		float const table_x = frac( octave_scale * x ) * 8.0f;
		float const table_y = frac( octave_scale * y ) * 8.0f;

		u32 const x0 = math::floor( table_x );
		u32 const x1 = math::min<u32>( x0 + 1, 7u );

		u32 const y0 = math::floor( table_y );
		u32 const y1 = math::min<u32>( y0 + 1, 7u );

		float const alpha_x = frac( table_x );
		float const alpha_y = frac( table_y );

		result +=
			(aperiodic_noise_table[y0][x0] * (1.0f - alpha_x) + aperiodic_noise_table[y0][x1] * alpha_x) * (1.0f - alpha_y) +
			(aperiodic_noise_table[y1][x0] * (1.0f - alpha_x) + aperiodic_noise_table[y1][x1] * alpha_x) * alpha_y;
	}

	result /= static_cast<float>( num_octaves );

	return math::clamp_r( result, 0.0f, 1.0f );
}

} // namespace render
} // namespace vostok
