////////////////////////////////////////////////////////////////////////////
//	Created		: 23.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_RTP_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"rtp"
#include <vostok/extensions.h>

#include "rtp_memory.h"

namespace vostok {
	namespace render {
		namespace debug {
			class renderer;
		} // namespace debug
	} // namespace render
} //  namespace vostok 



#endif // #ifndef PCH_H_INCLUDED