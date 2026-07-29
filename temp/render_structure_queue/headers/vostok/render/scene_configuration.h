////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_CONFIGURATION_H_INCLUDED
#define RENDER_SCENE_CONFIGURATION_H_INCLUDED

namespace vostok {
namespace render {

struct scene_configuration {
	inline		scene_configuration	( ) { /* no source */ }

	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 0 } )*/	m_create_terrain;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 1 } )*/	m_create_particle_world;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 2 } )*/	m_has_clouds;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 3 } )*/	m_create_speedtree_world;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 4 } )*/	m_create_grass_world;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 5 } )*/	m_sky_enabled;
	/* 0x0000 */	unhandled type /* Bitfield( BitfieldType { underlying_type: TypeIndex( 0x30 ), length: 1, position: 6 } )*/	m_use_occlusion_culling;
}; // struct scene_configuration

STATIC_SIZE_ASSERT(scene_configuration, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_CONFIGURATION_H_INCLUDED
