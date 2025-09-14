////////////////////////////////////////////////////////////////////////////
//	Created		: 26.05.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_DIALOG_EDITOR_API_H_INCLUDED
#define VOSTOK_DIALOG_EDITOR_API_H_INCLUDED

#define VOSTOK_DIALOG_EDITOR_API

namespace vostok {
namespace dialog_editor {
	typedef memory::doug_lea_allocator_type allocator_type;

} // namespace dialog_editor
} // namespace vostok

#pragma make_public (vostok::dialog_editor::allocator_type)

#endif // #ifndef VOSTOK_DIALOG_EDITOR_API_H_INCLUDED