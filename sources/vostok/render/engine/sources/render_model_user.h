#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_USER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_USER_H_INCLUDED

#include "render_model_instance_impl.h"
#include "render_surface.h"
#include "render_surface_instance.h"

namespace vostok {

namespace memory {

class chunk_reader;

} // namespace memory

namespace render {

class material_effects_instance_cook_data;
class untyped_buffer;
struct surface_stats;

class user_render_surface : public render_surface {
public:
	user_render_surface( ) { }
	virtual ~user_render_surface( ) { }

	virtual void load_from_chunk_data( memory::chunk_reader& chunk ) = 0;
	virtual bool is_dynamic( ) const { return true; }

protected:
	void material_ready(
		resources::queries_result&			data,
		material_effects_instance_cook_data*	cook_data,
		char*								material_name
	);
};

STATIC_SIZE_ASSERT( user_render_surface, 0x9C );

class user_render_surface_editable : public user_render_surface {
public:
	user_render_surface_editable( ) { }
	virtual ~user_render_surface_editable( ) { }
	virtual void load_from_chunk_data( memory::chunk_reader& chunk );

	untyped_buffer* m_vb;
};

STATIC_SIZE_ASSERT( user_render_surface_editable, 0xA0 );

class user_render_surface_wire : public user_render_surface {
public:
	user_render_surface_wire( ) { }
	virtual ~user_render_surface_wire( ) { }
	virtual void load_from_chunk_data( memory::chunk_reader& chunk );
};

STATIC_SIZE_ASSERT( user_render_surface_wire, 0x9C );

class user_render_model_instance : public render_model_instance_impl {
public:
	user_render_model_instance( ) { }
	virtual ~user_render_model_instance( ) { }

	void assign_surface( user_render_surface* surface );
	void get_surfaces(
		float4x4 const*						mat_vp,
		float3 const*						view_pos,
		vector< render_surface_instance* >&	dest,
		bool								visible_only,
		u8									lod_id,
		u32									surface_flags
	);
	void get_surface_stats( u32 surface_id, surface_stats& stats ) const;
};

typedef resources::resource_ptr<
	user_render_model_instance,
	resources::unmanaged_intrusive_base
> user_mesh_instance_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_USER_H_INCLUDED
