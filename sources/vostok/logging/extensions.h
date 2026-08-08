////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_EXTENSIONS_H_INCLUDED
#define VOSTOK_LOGGING_EXTENSIONS_H_INCLUDED

#if VOSTOK_CORE_BUILDING
#	define VOSTOK_LOG_MODULE_INITIATOR			"core"
#endif // #if VOSTOK_CORE_BUILDING

#ifndef VOSTOK_LOG_MODULE_INITIATOR
#	error you should define VOSTOK_LOG_MODULE_INITIATOR macro before including this header
#endif // #ifndef VOSTOK_LOG_MODULE_INITIATOR

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vostok/logging/api.h>
#include <vostok/logging/format_specifier.h>
#include <vostok/logging/format.h>

namespace vostok {
namespace logging {

extern format_specifier		format_thread_id;
extern format_specifier		format_initiator;
extern format_specifier		format_time;
extern format_specifier		format_verbosity;
extern format_specifier		format_message;

} // namespace logging
} // namespace vostok

#include <vostok/logging/logger.h>
#include <vostok/logging/macros.h>

#endif // #ifndef VOSTOK_LOGGING_EXTENSIONS_H_INCLUDED
