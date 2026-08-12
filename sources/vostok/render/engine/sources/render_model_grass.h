#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_GRASS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_GRASS_H_INCLUDED

#include <vostok/math_color.h>
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include "render_model.h"
#include "render_surface.h"

namespace vostok {
namespace render {

struct grass_source_vertex {
	float3 position;
	math::color normal;
	math::color tangent;
	math::color binormal;
	float2 uv;
};

STATIC_SIZE_ASSERT( grass_source_vertex, 0x20 );

class grass_render_surface : public render_surface {
typedef render_surface super;

public:
	grass_render_surface( );
	virtual ~grass_render_surface( );

	virtual void load( configs::binary_config_value const& properties, memory::chunk_reader& chunk );
	virtual bool is_dynamic( ) const { return true; }

	grass_source_vertex* m_vertices;
	u32 m_num_vertices;
	u16* m_indices;
	u32 m_num_indices;
};

STATIC_SIZE_ASSERT( grass_render_surface, 0xAC );

class grass_render_model : public render_model {
	typedef render_model super;
	typedef grass_render_surface* child;

public:
	grass_render_model( ) :
		m_l0( 0 ),
		m_l1( 0 ),
		m_l2( 0 )
	{
	}

	virtual void set_children( render_surface** children, u8 count, model_lods_descriptor* lods );

	child m_l0;
	child m_l1;
	child m_l2;
};

STATIC_SIZE_ASSERT( grass_render_model, 0x148 );

typedef resources::resource_ptr<
	grass_render_model,
	resources::unmanaged_intrusive_base
> grass_render_model_ptr;

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_RENDER_MODEL_GRASS_H_INCLUDED
