#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

class render_surface;
struct model_locator_item;

struct model_lods_descriptor {
	model_lods_descriptor( )
	{
		m_lod_calc_type = 0;
		m_lod_params_default = true;
	}

	u8		m_lod_surfaces_count[3];
	pbyte	m_lod_surfaces[3];
	u8		m_lod_calc_type;
	float	m_lod_custom_params[3];
	bool	m_lod_params_default;
};

STATIC_SIZE_ASSERT( model_lods_descriptor, 0x24 );

class render_model : public resources::unmanaged_resource {
	typedef render_surface* children;
public:
	render_model( );
	virtual ~render_model( );

	virtual void load_properties( configs::binary_config_value const& properties );
	virtual void set_children( render_surface** children_in, u8 count, model_lods_descriptor* lods );

	math::aabb				m_aabbox;
	virtual bool get_locator( pcstr locator_name, model_locator_item& result ) const;

protected:
	model_locator_item*		m_locators;
	u16						m_locators_count;

public:
	model_lods_descriptor*	m_lods_descriptor;
	children*				m_childs;
	u8						m_childs_count;
};

STATIC_SIZE_ASSERT( render_model, 0x138 );

typedef resources::resource_ptr<
	render_model,
	resources::unmanaged_intrusive_base
> render_model_ptr;

} // namespace render
} // namespace vostok

#include "render_surface.h"
#include "render_model_instance_impl.h"

namespace vostok {
namespace render {

inline render_surface::render_surface( ) :
	m_aabbox( math::create_zero_aabb( ) ),
	m_streaming_texture_factor( 10000.0f )
{
}

inline void render_model_instance_impl::set_transform( float4x4 const& transform )
{
	m_transform = transform;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_H_INCLUDED
