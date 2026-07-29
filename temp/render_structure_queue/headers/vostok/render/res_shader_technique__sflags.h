////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_SHADER_TECHNIQUE_SFLAGS_H_INCLUDED
#define RENDER_RES_SHADER_TECHNIQUE_SFLAGS_H_INCLUDED

namespace vostok {
namespace render {

struct res_shader_technique::sflags {

	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 2, position: 0 } )*/	priority;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 2 } )*/	strict_b2f;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 3 } )*/	has_emissive;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 4 } )*/	has_distort;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x75 ), length: 1, position: 5 } )*/	has_wmark;
}; // struct res_shader_technique::sflags

STATIC_SIZE_ASSERT(res_shader_technique::sflags, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_SHADER_TECHNIQUE_SFLAGS_H_INCLUDED
