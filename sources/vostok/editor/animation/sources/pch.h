////////////////////////////////////////////////////////////////////////////
//	Created		: 26.05.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_ANIMATION_EDITOR_BUILDING

#define VOSTOK_LOG_MODULE_INITIATOR	"animation_editor"
#include <vostok/editor/base/editor_extensions.h>

#pragma managed( push, off )
#	include "animation_editor_memory.h"
#	include <vostok/render/world.h>
#pragma managed( pop )

#include <vostok/editor/base/images/images16x16.h>

namespace vostok {
namespace animation_editor {
#	include <vostok/editor/base/unmanaged_string.h>
} // namespace animation_editor
} // namespace vostok


#pragma make_public (vostok::animation_editor::allocator_type)
#pragma make_public (vostok::render::world)

#endif // #ifndef PCH_H_INCLUDED