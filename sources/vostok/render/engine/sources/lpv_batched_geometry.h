// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_LPV_BATCHED_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LPV_BATCHED_GEOMETRY_H_INCLUDED
#include "batched_geometry.h"

namespace vostok {
namespace render {

struct lpv_batched_geometry : public batched_geometry<lpv_vertex> {
	lpv_batched_geometry( );

	virtual void add_vertex(
		batched_vertex_source const&	in_vertex,
		float3 const&				not_modified_position
	) override;

	virtual void build(
		vector<render_model_instance_impl_ptr>& model_instances
	) override;
};

STATIC_SIZE_ASSERT( lpv_batched_geometry, 0xCC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_LPV_BATCHED_GEOMETRY_H_INCLUDED
