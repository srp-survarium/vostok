#include "pch.h"
#include "model_manager.h"
#include <vostok/render/engine/model_format.h>
#include "render_model.h"
#include "render_surface.h"
#include "render_model_cooker.h"
#include "render_model_static.h"
#include "render_model_grass.h"
#include "render_model_skeleton.h"
#include "render_model_user.h"
#include "skeleton_mesh_gpu_skinning_1weight.h"
#include "skeleton_mesh_gpu_skinning_2weights.h"
#include "skeleton_mesh_gpu_skinning_3weights.h"
#include "skeleton_mesh_gpu_skinning_4weights.h"

namespace vostok {
namespace render {
namespace model_factory {

void destroy_render_model( render_model* model )
{
	// FUNCTION BODY[0x781940]
	DELETE( model );
}

void destroy_render_surface( render_surface* surface )
{
	// FUNCTION BODY[0x781980]
	DELETE( surface );
}

u16 get_model_type( memory::chunk_reader& chunk )
{
	// FUNCTION BODY[0x7819c0]
	memory::reader	ogf = chunk.open_reader( model_chunk_header );
	model_header		header;
	ogf.r			(&header, sizeof(header), sizeof(header));
	return			header.type;
}

render_surface* create_render_surface( u16 type )
{
	// FUNCTION BODY[0x781a90]
	render_surface* result	= NULL;
	switch(type)
	{
	case mt_static_submesh:
		result = NEW(static_render_surface)(false);
		break;

	case mt_static_submesh_colored:
		result = NEW(static_render_surface)(true);
		break;

	case mt_skinned_submesh_4w:
		result	= NEW(skeleton_mesh_gpu_skinning_4weights)();
		break;
	case mt_skinned_submesh_3w:
		result	= NEW(skeleton_mesh_gpu_skinning_3weights)();
		break;
	case mt_skinned_submesh_2w:
		result	= NEW(skeleton_mesh_gpu_skinning_2weights)();
		break;
	case mt_skinned_submesh_1w:
		result	= NEW(skeleton_mesh_gpu_skinning_1weight)();
		break;
	case mt_user_mesh_editable:
		result = NEW(user_render_surface_editable)();
		break;
	case mt_user_mesh_wire:
		result = NEW(user_render_surface_wire)();
		break;
	case mt_grass_mesh:
		result = NEW(grass_render_surface)();
		break;

	default:
		NODEFAULT();
	};
	return result;
}

render_model* create_render_model( u16 type )
{
	// FUNCTION BODY[0x781a00]
	render_model* result	= NULL;
	switch(type)
	{
	case mt_static_mesh:
		result = NEW(static_render_model)();
		break;

	case mt_grass_mesh:
		result = NEW(grass_render_model)();
		break;

	case mt_skinned_mesh:
		result = NEW(skeleton_render_model)();
		break;
	default:
		NODEFAULT();
	};

	return result;
}

} // namespace model_factory
} // namespace render
} // namespace vostok
