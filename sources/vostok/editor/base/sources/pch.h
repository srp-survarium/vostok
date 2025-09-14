////////////////////////////////////////////////////////////////////////////
//	Created		: 02.06.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_EDITOR_BASE_BUILDING

#define VOSTOK_LOG_MODULE_INITIATOR	"editor_base"
#include <vostok/editor/base/editor_extensions.h>

#include "property_holder_delegates_types.h"

#pragma managed( push, off )
#include "editor_base_memory.h"
#pragma managed( pop )

namespace vostok {
namespace editor_base {
#	include <vostok/editor/base/unmanaged_string.h>
} // namespace editor_base
} // namespace vostok

#include "make_public_helper.h"

#endif // #ifndef PCH_H_INCLUDED
