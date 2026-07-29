////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LIGHT_LIGHT_FLAGS_H_INCLUDED
#define RENDER_LIGHT_LIGHT_FLAGS_H_INCLUDED

namespace vostok {
namespace render {

struct light::light_flags {

	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 4, position: 0 } )*/	type;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 4 } )*/	is_static;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 5 } )*/	does_cast_shadows;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 6 } )*/	is_hud_mode;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 7 } )*/	cast_shadow_in_x;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 8 } )*/	cast_shadow_in_neg_x;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 9 } )*/	cast_shadow_in_y;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 10 } )*/	cast_shadow_in_neg_y;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 11 } )*/	cast_shadow_in_z;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 12 } )*/	cast_shadow_in_neg_z;
}; // struct light::light_flags

STATIC_SIZE_ASSERT(light::light_flags, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LIGHT_LIGHT_FLAGS_H_INCLUDED
