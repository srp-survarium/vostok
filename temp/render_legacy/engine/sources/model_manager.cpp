////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "model_manager.h"
#include <vostok/render/engine/model_format.h>
#include "render_model_static.h"
#include "render_model_grass.h"
#include "terrain_render_model.h"
#include "skeleton_mesh_gpu_skinning_4weights.h"
#include "render_model_user.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_geometry.h>
#include <vostok/render/core/custom_config.h>

namespace vostok {
namespace render {
namespace model_factory {

// REMAINDER: get_surface_type has no canonical counterpart (merged into
// get_model_type in the shipped tree).




u16 get_surface_type(memory::chunk_reader& chunk)
{
	memory::reader	ogf = chunk.open_reader( model_chunk_header );
	model_header		header;
	ogf.r			(&header, sizeof(header), sizeof(header));
	return			header.type;
}



} // namespace model_factory
} // namespace render 
} // namespace vostok