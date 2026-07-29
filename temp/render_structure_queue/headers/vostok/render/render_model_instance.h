////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_MODEL_INSTANCE_H_INCLUDED
#define RENDER_RENDER_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::render::culling::possible_sectors_holder;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class vostok::render::model_locator_item;
class vostok::render::surface_stats;

namespace vostok {
namespace render {

class render_model_instance : public resources::unmanaged_resource {
public:
	inline			render_model_instance	( ) { /* no source */ }

	virtual	void	update					( ) { /* no source */ }

	virtual	bool	get_locator				( pcstr __formal, model_locator_item& __formal ) const { /* no source */ }
	virtual	u32		get_surfaces_count		( ) const = 0;
	virtual	u32		get_surfaces_count		( u32 arg_0 ) const = 0;
	virtual	void	get_surface_stats		( u32 arg_0, surface_stats& arg_1 ) const = 0;
	virtual	void	get_bind_pose			( float4x4* __formal, u32 __formal ) const { /* no source */ }
	inline	culling::possible_sectors_holder const*	get_sectors_holder		( ) const { /* no source */ }

	virtual	void	set_lod_params			(
						u8			__formal,
						bool		__formal,
						float		__formal,
						float		__formal,
						float		__formal
					) { /* no source */ }

protected:
	virtual			~render_model_instance	( ) = 0;

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	culling::possible_sectors_holder*		m_sectors_holder;
	/* 0x010c */	u8										m_current_lod_index;
}; // class render_model_instance

STATIC_SIZE_ASSERT(render_model_instance, 0x110);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_MODEL_INSTANCE_H_INCLUDED
