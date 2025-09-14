////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef DEBUG_INTERNAL_H_INCLUDED
#define DEBUG_INTERNAL_H_INCLUDED

struct _SYSTEMTIME;

namespace vostok {
namespace core {
namespace debug {

void set_thread_stack_guarantee				( );

namespace bugtrap {
	void setup_unhandled_exception_handler	( );
} // namespace bugtrap

namespace detail {
	void generate_file_name					(
			string_path& file_name,
			_SYSTEMTIME const& date_time,
			pcstr const application_id,
			pcstr const report_id,
			pcstr const extension
		);
} // namespace detail

VOSTOK_CORE_API u32	build_station_build_id	( );

} // namespace debug
} // namespace core
} // namespace vostok

#endif // #ifndef DEBUG_INTERNAL_H_INCLUDED