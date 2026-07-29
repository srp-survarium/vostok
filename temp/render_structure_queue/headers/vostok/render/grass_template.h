////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_TEMPLATE_H_INCLUDED
#define RENDER_GRASS_TEMPLATE_H_INCLUDED

/* INCLUDES */
class vostok::render::vector<vostok::render::grass_instance *>;
typedef vostok::resources::resource_ptr<vostok::render::grass_render_model,vostok::resources::unmanaged_intrusive_base>
	vostok::render::grass_render_model_ptr;
class vostok::render::grass_render_model;

namespace vostok {
namespace render {

struct grass_template {
					grass_template	( const u32 in_index, grass_render_model_ptr const& in_model_ptr );
					~grass_template	( );

	inline	void	apply			( ) { /* no source */ }

private:
	/* 0x0000 */	grass_render_model_ptr			m_render_model;
	/* 0x0004 */	u32								m_index;
	/* 0x0008 */	vector< grass_instance* >		m_instances;
	/* 0x0014 */	float3							m_sizes;
}; // struct grass_template

STATIC_SIZE_ASSERT(grass_template, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_TEMPLATE_H_INCLUDED
