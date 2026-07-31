#ifndef VOSTOK_RENDER_ENGINE_GRASS_TEMPLATE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GRASS_TEMPLATE_H_INCLUDED

#include <vostok/math_float3.h>
#include <vostok/render/core/memory.h>
#include "render_model_grass.h"

namespace vostok {
namespace render {

struct grass_instance;

struct grass_template {
	grass_template( u32 in_index, grass_render_model_ptr const& in_model_ptr );
	~grass_template( );

	void apply( )
	{
	}

private:
	friend struct grass_world;

	grass_render_model_ptr m_render_model;
	u32 m_index;
	vector<grass_instance*> m_instances;
	float3 m_sizes;
};

STATIC_SIZE_ASSERT( grass_template, 0x20 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_GRASS_TEMPLATE_H_INCLUDED
