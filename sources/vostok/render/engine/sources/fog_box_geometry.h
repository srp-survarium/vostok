#ifndef VOSTOK_RENDER_ENGINE_FOG_BOX_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_FOG_BOX_GEOMETRY_H_INCLUDED

#include "res_geometry.h"

namespace vostok {
namespace render {

struct fog_box_geometry {
	fog_box_geometry( );
	void render( );
	~fog_box_geometry( ) { }

	res_geometry_ptr m_geometry;
};

STATIC_SIZE_ASSERT( fog_box_geometry, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_FOG_BOX_GEOMETRY_H_INCLUDED
