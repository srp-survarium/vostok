////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\ENGINE\SOURCES\AAB_RECT_INLINE_H_INCLUDED
#define VOSTOK\RENDER\ENGINE\SOURCES\AAB_RECT_INLINE_H_INCLUDED

namespace vostok {
namespace render {
namespace culling {

// STATE[STUB]
inline bool aab_rect::contains( aab_rect const& another ) const
{
	return false;

	// FUNCTION BODY[0x8a470]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x8a471>|0x001|+0x0b1:'52'
	// <0x8a522>|0x0b2|-0x002:'52'
	// <0x8a520>|0x0b0|+0x007:'53'
	// ******
}

// STATE[STUB]
inline aab_rect get_intersection_rect( aab_rect const& left, aab_rect const& right )
{
	// FUNCTION BODY[0x8a530]: 7
	// <0>
	// <0x8a534>|0x004|+0x016:'77'
	// <0x8a54a>|0x01a|+0x011:'78'
	// <0x8a55b>|0x02b|+0x016:'79'
	// <0x8a571>|0x041|+0x017:'80'
	// <0x8a588>|0x058|+0x01b:'81'
	// <0>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::culling::portal_sector_system::quad*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\ENGINE\SOURCES\AAB_RECT_INLINE_H_INCLUDED
