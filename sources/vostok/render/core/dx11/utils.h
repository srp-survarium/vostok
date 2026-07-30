#ifndef VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED

#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {
namespace utils {

template < typename list_type >
bool calc_lists_diff_range(
	list_type const&,
	list_type const&,
	u32&,
	u32&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x12ce30] for res_texture_list
	return false;
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

} // namespace utils
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_UTILS_H_INCLUDED
