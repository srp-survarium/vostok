////////////////////////////////////////////////////////////////////////////
//	Created		: 12.07.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "editor_creator.h"
#include "particle_editor.h"
#include <vostok/render/world.h>

namespace vostok {
namespace particle_editor {

vostok::editor_base::tool_window^ editor_creator::create_editor( System::String^ resources_path, 
																render::world& render_world, 
																vostok::ui::world& ui_world,
																vostok::editor_base::tool_window_holder^ h )
{
	particle_editor^ ed				= gcnew particle_editor(resources_path, render_world, ui_world, h);
	return			ed;
}

void editor_creator::set_memory_allocator(vostok::particle_editor::allocator_type* allocator)
{
	g_allocator = allocator;
}

} //namespace particle_editor
} //namespace vostok