////////////////////////////////////////////////////////////////////////////
//	Created		: 12.07.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_PARTICLE_EDITOR_BUILDING

#define VOSTOK_LOG_MODULE_INITIATOR	"particle_editor"
#include <vostok/editor/base/editor_extensions.h>


#pragma managed( push, off )
#include "particle_editor_memory.h"
#pragma managed( pop )

namespace vostok {
namespace particle_editor {
#include <vostok/editor/base/unmanaged_string.h>
} // namespace particle_editor
} // namespace vostok

#include <vostok/editor/base/images/images16x16.h>
#include <vostok/render/facade/editor_renderer.h>

#pragma make_public (vostok::particle_editor::allocator_type)
#include <vostok/render/world.h>
#pragma make_public (vostok::render::world)

#endif // #ifndef PCH_H_INCLUDED