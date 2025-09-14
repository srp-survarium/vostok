////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_EDITOR_BUILDING

#define VOSTOK_LOG_MODULE_INITIATOR	"editor"
#include <vostok/editor/base/editor_extensions.h>

#pragma managed( push, off )
#	include "editor_memory.h"
#pragma managed( pop )

namespace vostok {
namespace editor {
#	include <vostok/editor/base/unmanaged_string.h>
} // namespace editor
} // namespace vostok

#include <vostok/editor/base/images/images16x16.h>
#include "images/folder_icons.h"

RegistryKey^ get_sub_key(RegistryKey^ root, System::String^ name);

namespace vostok {
	namespace render {
		namespace editor {
			class renderer;
		} // namespace editor
	} // namespace render
} // namespace vostok

#endif // #ifndef PCH_H_INCLUDED