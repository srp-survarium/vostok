#ifndef VOSTOK_RENDER_FACADE_MATERIAL_EFFECTS_INSTANCE_COOK_DATA_H_INCLUDED
#define VOSTOK_RENDER_FACADE_MATERIAL_EFFECTS_INSTANCE_COOK_DATA_H_INCLUDED

#include <vostok/render/facade/cull_mode.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

struct material_effects_instance_cook_data {
	~material_effects_instance_cook_data( ) { }

	material_effects_instance_cook_data(
		enum_vertex_input_type				in_vertex_input_type,
		resources::unmanaged_resource_ptr	in_material,
		bool								in_delete_in_cook = true,
		enum_cull_mode						in_cull_mode = cull_mode_back
	) :
		vertex_input_type	( in_vertex_input_type ),
		material			( in_material ),
		cull_mode			( in_cull_mode ),
		delete_in_cook		( in_delete_in_cook )
	{
	}

	enum_vertex_input_type				vertex_input_type;
	resources::unmanaged_resource_ptr	material;
	enum_cull_mode						cull_mode;
	bool								delete_in_cook;
};

STATIC_SIZE_ASSERT( material_effects_instance_cook_data, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_MATERIAL_EFFECTS_INSTANCE_COOK_DATA_H_INCLUDED
