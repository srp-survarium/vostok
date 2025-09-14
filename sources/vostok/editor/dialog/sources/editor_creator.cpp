////////////////////////////////////////////////////////////////////////////
//	Created		: 10.06.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "editor_creator.h"
#include "dialog_editor.h"
namespace vostok{
namespace dialog_editor{

vostok::editor_base::tool_window^ editor_creator::create_editor( System::String^ resources_path, 
															  vostok::editor_base::tool_window_holder^ h )
{
	dialog_editor_impl^ ed		= gcnew dialog_editor_impl( resources_path, h );
	return				ed;
}

void editor_creator::set_memory_allocator(vostok::dialog_editor::allocator_type* allocator)
{
	g_allocator = allocator;
}

} //namespace dialog_editor
} //namespace vostok

