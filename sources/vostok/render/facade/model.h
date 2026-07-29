#ifndef VOSTOK_RENDER_FACADE_MODEL_H_INCLUDED
#define VOSTOK_RENDER_FACADE_MODEL_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_float4x4.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

namespace culling {

class possible_sectors_holder;

} // namespace culling

struct model_locator_item {
	model_locator_item( ) { }

	char		m_name[32];
	float4x4	m_offset;
	u16			m_bone;
};

STATIC_SIZE_ASSERT( model_locator_item, 0x64 );

struct surface_stats {
	surface_stats( ) { }

	u32					vcount;
	u32					tricount;
	fixed_string< 260 >	material;
};

STATIC_SIZE_ASSERT( surface_stats, 0x118 );

class render_model_instance : public resources::unmanaged_resource {
public:
	render_model_instance( ) :
		m_sectors_holder		( 0 ),
		m_current_lod_index	( 0 )
	{
	}

	virtual void update( )
	{
		// STATE[STUB]
		// FUNCTION BODY[0x4f210]
	}

	virtual bool get_locator( pcstr, model_locator_item& ) const
	{
		// STATE[STUB]
		// FUNCTION BODY[0xb4f50]
		return false;
	}

	virtual u32 get_surfaces_count( ) const = 0;
	virtual u32 get_surfaces_count( u32 lod_id ) const = 0;
	virtual void get_surface_stats( u32 surface_id, surface_stats& stats ) const = 0;

	virtual void get_bind_pose( float4x4*, u32 ) const
	{
		// STATE[STUB]
		// FUNCTION BODY[0x38090]
	}

	culling::possible_sectors_holder const* get_sectors_holder( ) const
	{
		return m_sectors_holder;
	}

	virtual void set_lod_params( u8, bool, float, float, float )
	{
		// STATE[STUB]
		// FUNCTION BODY[0x61980]
	}

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( render_model_instance )

	culling::possible_sectors_holder*	m_sectors_holder;
	u8									m_current_lod_index;
};

STATIC_SIZE_ASSERT( render_model_instance, 0x110 );

typedef resources::resource_ptr<
	render_model_instance,
	resources::unmanaged_intrusive_base
> render_model_instance_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_MODEL_H_INCLUDED
