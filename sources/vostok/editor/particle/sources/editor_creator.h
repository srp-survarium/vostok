////////////////////////////////////////////////////////////////////////////
//	Created		: 12.07.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#ifndef EDITOR_CREATOR_H_INCLUDED
#define EDITOR_CREATOR_H_INCLUDED

#include <vostok/ui/world.h>

#pragma make_public(vostok::ui::world)

namespace vostok {

namespace render{
	namespace editor{ class renderer;}
}

namespace particle_editor {

public ref class editor_creator
{
public:
	static vostok::editor_base::tool_window^	create_editor	( System::String^ resources_path, 
																render::world& renderer_world, 
																vostok::ui::world& ui_world,
																vostok::editor_base::tool_window_holder^ h );

	static void								set_memory_allocator	( allocator_type* allocator );
	
}; // ref class editor_creator

} // namespace particle_editor
} // namespace vostok
#endif // #ifndef EDITOR_CREATOR_H_INCLUDED
