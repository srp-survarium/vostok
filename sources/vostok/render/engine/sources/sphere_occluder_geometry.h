// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SPHERE_OCCLUDER_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPHERE_OCCLUDER_GEOMETRY_H_INCLUDED
#include "res_geometry.h"

namespace vostok {
namespace render {

struct sphere_occluder_geometry {
	sphere_occluder_geometry( );
	void render( );

private:
	res_geometry_ptr m_geometry;
};

STATIC_SIZE_ASSERT( sphere_occluder_geometry, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SPHERE_OCCLUDER_GEOMETRY_H_INCLUDED
