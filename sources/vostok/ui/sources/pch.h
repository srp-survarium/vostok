#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_UI_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"ui"
#include <vostok/extensions.h>

//#include "ui_base.h"
//#include <vostok/ui/api.h>
//#include <vostok/ui/ui.h>

using namespace vostok;
//using namespace vostok::ui;

#endif // #ifndef PCH_H_INCLUDED