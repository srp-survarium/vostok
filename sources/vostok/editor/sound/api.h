////////////////////////////////////////////////////////////////////////////
//	Created		: 08.07.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_EDITOR_API_H_INCLUDED
#define VOSTOK_SOUND_EDITOR_API_H_INCLUDED

#define VOSTOK_SOUND_EDITOR_API

static vostok::command_line::key	s_sound_editor_start	("start_sound_editor", "", "", "start sound editor");

namespace vostok {
namespace sound_editor {
	typedef memory::doug_lea_allocator_type allocator_type;

} // namespace sound_editor
} // namespace vostok


#endif // #ifndef VOSTOK_SOUND_EDITOR_API_H_INCLUDED