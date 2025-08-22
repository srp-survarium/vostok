////////////////////////////////////////////////////////////////////////////
//	Created		: 08.07.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_SOUND_EDITOR_BUILDING

#define VOSTOK_LOG_MODULE_INITIATOR	"sound_editor"
#include <vostok/editor/base/editor_extensions.h>

#pragma managed( push, off )
#include "sound_editor_memory.h"
#pragma managed( pop )

namespace vostok {
namespace sound_editor {
#include <vostok/editor/base/unmanaged_string.h>
} // namespace sound_editor
} // namespace vostok

#include <vostok/editor/base/images/images16x16.h>
#include <vostok/sound/world.h>
#include <vostok/render/world.h>

#pragma make_public (vostok::sound_editor::allocator_type)
#pragma make_public (vostok::sound::world)
#pragma make_public (vostok::render::world)

#endif // #ifndef PCH_H_INCLUDED