#ifndef VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED

#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {
namespace utils {

template < typename list_type >
bool calc_lists_diff_range( list_type const& first, list_type const& second, u32 &min, u32 &max)
{
	// FUNCTION BODY[0x12ce30] for res_texture_list
	list_type::const_iterator	it	= first.begin();
	list_type::const_iterator	end = first.end();

	list_type::const_iterator	it_oth	= second.begin();
	list_type::const_iterator	end_oth = second.end();

	min = max = 0;
	for( ; it != end && it_oth != end_oth; ++it, ++it_oth, ++min)
		if( *it != *end_oth)
			break;


	if( first.size() != second.size())
	{
		max = math::max( first.size(), second.size());
		return min != max;
	}

	int ind = min;
	for( ; it != end && it_oth != end_oth; ++it, ++it_oth, ++ind)
	{
		if( *it != *end_oth)
		{
			max = ind+1;
//			*it = *it_oth;
		}
	}

	return min != max;
}

u32 get_format_4x4_pixel_size( DXGI_FORMAT format );

u32 calc_surface_size(
	u32 width,
	u32 height,
	DXGI_FORMAT format,
	u32 row_min_pitch,
	u32& row_pitch
);

inline u32 calc_surface_size(
	u32 width,
	u32 height,
	DXGI_FORMAT format,
	u32 row_min_pitch = 0
)
{
	u32 row_pitch = 0;
	return calc_surface_size(
		width,
		height,
		format,
		row_min_pitch,
		row_pitch
	);
}

// Calculates logarithm with base 2 only for powers of 2.
inline u32 log_2( u32 val )
{
#pragma warning( push)
#pragma warning( disable:4146)
#pragma warning( disable:4554)

	ASSERT( (val & -val) != 0 || val == 0, "The function works only for powers of 2." );

	float f = (float)( val & -val); // cast the least significant bit in v to a float
	return (*(u32*)&f >> 23) - 0x7f;	// cast the least significant bit in v to a float

#pragma warning( pop)
}

} // namespace utils
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED
