#ifndef VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED

#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {
namespace utils {

template < typename list_type >
bool calc_lists_diff_range( list_type const& first, list_type const& second, u32 &min, u32 &max);

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
);

// Calculates logarithm with base 2 only for powers of 2.
inline u32 log_2( u32 val );

} // namespace utils
} // namespace render
} // namespace vostok

#include <vostok/render/core/utils_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED
