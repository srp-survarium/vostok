#ifndef VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_H_INCLUDED

// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (the batched_geometry family is new-in-target) - matcher-phase work.
#include <boost/function.hpp>
#include <vostok/math_aabb.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/memory.h>

#include "batched_vertex_source.h"
#include "geometry_batch.h"
#include "lpv_vertex.h"
#include "material_effects_instance.h"
#include "render_model_instance_impl.h"
#include "shadow_vertex.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace vostok {
namespace render {

class renderer_context;

struct batched_geometry_interface {
	virtual void add_data(
		batched_vertex_source const*,
		u32,
		u16 const*,
		u32,
		float4x4 const&,
		material_effects_instance_ptr const&
	) = 0;

	virtual void build( vector<render_model_instance_impl_ptr>& ) = 0;

protected:
	batched_geometry_interface( )
	{
	}
};

STATIC_SIZE_ASSERT( batched_geometry_interface, 0x4 );

template <typename Vertex>
struct batched_geometry : public batched_geometry_interface {
	void for_each_batch_render(
		renderer_context*,
		boost::function<void ( geometry_batch const& )> const&,
		boost::function<void ( geometry_batch const& )> const&
	);

	void for_each_batch_visible_render_first_and_remove(
		boost::function<void ( geometry_batch const& )> const&,
		boost::function<void ( geometry_batch const& )> const&,
		u32,
		u32
	);

	u32 prepare_visible_batches( renderer_context*, float3 const&, bool, u32 );
	u32 get_num_visible_batches( u32 index ) const;

	batched_geometry( );

protected:
	batched_geometry(
		D3D11_INPUT_ELEMENT_DESC const*,
		u32,
		u32 in_batched_geometry_max_vertices_count
	);

	~batched_geometry( );

	virtual void add_vertex(
		batched_vertex_source const&,
		float3 const&
	) = 0;

	virtual void add_data(
		batched_vertex_source const*,
		u32,
		u16 const*,
		u32,
		float4x4 const&,
		material_effects_instance_ptr const&
	) override;

	void finalize_batch( );
	void invalidate( );

	vector<geometry_batch>			m_geometry_batches;
	vector<geometry_batch>			m_visible_geometry_batches[8];
	res_declaration_ptr				m_layout;
	u32								m_batched_geometry_max_vertices_count;
	u32								m_num_visible_batches[8];
	vector<Vertex>					m_vertices;
	vector<u16>						m_indices;
	math::aabb						m_bbox;
	material_effects_instance_ptr	m_materail_effects_instance;
};

STATIC_SIZE_ASSERT( batched_geometry<lpv_vertex>, 0xCC );
STATIC_SIZE_ASSERT( batched_geometry<shadow_vertex>, 0xCC );

} // namespace render
} // namespace vostok

#include <vostok/render/engine/sources/batched_geometry_inline.h>

#endif // #ifndef VOSTOK_RENDER_ENGINE_BATCHED_GEOMETRY_H_INCLUDED
