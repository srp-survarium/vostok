////////////////////////////////////////////////////////////////////////////
//	Created		: 02.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_TERRAIN_BASE_H_INCLUDED
#define VOSTOK_RENDER_BASE_TERRAIN_BASE_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace render {

struct terrain_cook_user_data {
	scene_ptr	scene;
	pcstr		current_project_resource_path;
};


} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_TERRAIN_BASE_H_INCLUDED