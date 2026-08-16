////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/core/core.h>

#include "build_extensions.h"
#include "game_resman.h"
#include "memory.h"
#include "resources_manager.h"
#include "strings_shared_manager.h"
#include "timing.h"
#include "unmanaged_allocation_cook.h"
#include <locale.h>			// for setlocale
#include <vostok/compressor_ppmd.h>
#include <vostok/configs.h>
#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>
#include <vostok/core/engine.h>
#include <vostok/core_entry_point.h>
#include <vostok/core_test_suite.h>
#include <vostok/debug/debug.h>
#include <vostok/debug/log_callback.h>
#include <vostok/fs/path_string_utils.h>
#include <vostok/fs/synchronous_device_interface.h>
#include <vostok/logging/api.h>
#include <vostok/logging/format.h>
#include <vostok/memory_base_allocator.h>


#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable : 4995 )
	pvoid function_pointers[] = {
		&malloc,
		&calloc,
		&realloc,
		&free,
		&_malloc_crt,
		&_calloc_crt,
		&_realloc_crt,
		&_recalloc_crt,
		&_msize
	};
#	pragma warning( pop )
#endif // #ifdef _MSC_VER

using vostok::debug::error_mode;
using vostok::debug::bugtrap_usage;

static bool	s_initialized				= false;
static vostok::core::engine * s_engine	=	NULL;

vostok::uninitialized_reference<vostok::fs_new::synchronous_device_interface>	s_core_synchronous_device;

namespace vostok {
namespace debug {
	void			notify_xbox_debugger( pcstr message );
	void			postinitialize		( );
} // namespace debug

namespace core {
	bool initialized	( );

		// sushi@TODO
} // namespace core

#ifndef	MASTER_GOLD
namespace core {
namespace configs {
	void initialize		( vostok::core::engine& engine, pcstr lua_config_device_folder_to_save_to );
	void finalize		();
} // namespace configs
} // namespace core

#endif	// MASTER_GOLD

namespace threading {
	void preinitialize	( );
	void finalize		( );
	void initialize		( );
	void on_thread_spawn( tasks_awareness const task_awareness );
} // namespace threading

namespace memory {
extern doug_lea_allocator_type				g_log_allocator;
} // namespace memory

} // namespace vostok

void vostok::core::preinitialize		( core::engine *							engine,
									  logging::log_file_usage_enum const			log_file_usage,
									  pcstr const									command_line,
									  command_line::contains_application_bool const	command_line_contains_application,
									  pcstr	const									application,
									  pcstr	const									build_date
									)
{
	g_log_file_usage		= log_file_usage; // sushi@TODO: Might have been hidden with an inlined function
	s_engine				= engine;
	R_ASSERT				( !s_initialized, "you cannot preinitialize core when it has been initialized already" );

	using namespace			debug;
	command_line::initialize( engine, command_line, command_line_contains_application );

	debug::initialize		( s_engine );

	set_application_name	( application );

	core::platform::preinitialize	( );
	threading::preinitialize( );

	VOSTOK_CONSTRUCT_REFERENCE(s_core_synchronous_device, fs_new::synchronous_device_interface)
								(get_core_device_file_system( ), fs_new::watcher_enabled_false);

	memory::g_use_resources_manager	= engine->use_resources_manager( );
	memory::g_use_video_memory		= engine->use_video_memory( );

	memory::preinitialize	( );
	build::preinitialize	( build_date );

	fs_new::device_file_system_proxy	device(get_core_device_file_system(), fs_new::watcher_enabled_true); // sushi@NOTE: Deleted. This should be handled when `fs_new` or `core` is matched.

	g_log_format.set( logging::format_separator("{") +
							  logging::format_thread_id +
							  logging::format_time +
							  logging::format_separator("} [") +
							  logging::format_initiator +
							  logging::format_separator("] <") +
							  logging::format_verbosity +
							  logging::format_separator(">   ") +
							  logging::format_message );
}

bool vostok::core::initialized ( )
{
	return					s_initialized;
}

void vostok::core::initialize			(
		pcstr const lua_config_device_folder_to_save_to,
		pcstr const debug_thread_id,
		debug_initialization const debug_initialization
	)
{
	R_ASSERT				( !s_initialized, "you cannot initialize core when it has been initialized already" );

	if ( debug_initialization == perform_debug_initialization)
		debug::postinitialize	( );

	// for language-dependent strings
	setlocale				( LC_CTYPE, "" );

	threading::set_thread_name	( debug_thread_id, debug_thread_id );

	threading::initialize	( );

	LOG_INFO				( "working directory: '%s'", fs_new::get_current_directory().c_str() );
	LOG_INFO				( "resources directory: '%s'", s_engine->get_resources_path() );

#ifdef VOSTOK_STATIC_LIBRARIES
	command_line::check_keys( );
	command_line::handle_help_key( );
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

	timing::initialize		( );
	build::initialize		( s_engine );

	strings::initialize		( );


	fs_new::native_path_string replication_folder_string = core::user_data_directory( );
	replication_folder_string.append_with_conversion( "/replication" );
	pcstr const replication_folder	= replication_folder_string.c_str();

	VOSTOK_UNREFERENCED_PARAMETER				(replication_folder);

#ifndef MASTER_GOLD
	core::configs::initialize	( *s_engine, lua_config_device_folder_to_save_to );
#else // #ifndef MASTER_GOLD
	VOSTOK_UNREFERENCED_PARAMETER	( lua_config_device_folder_to_save_to );
#endif	// #ifndef MASTER_GOLD

	tasks::initialize		(	2 * threading::core_count(),	// tasks thread count
								64,								// user thread count
								threading::core_count(), //1,								// minimum active task thread count
								tasks::execute_while_wait_for_children_true,
								tasks::do_logging_false
							);
	threading::set_current_thread_affinity	( 0 );
	threading::on_thread_spawn	( threading::tasks_aware );
}

void	vostok::core::initialize_resources	(
							fs_new::asynchronous_device_interface &	hdd,
							fs_new::asynchronous_device_interface &	dvd,
							resources::enable_fs_watcher_bool		enable_fs_watcher
						)
{
	resources::initialize					( hdd, dvd, enable_fs_watcher );

	core_test_suite::singleton()->set_resources_path	( s_engine->get_resources_path() );

	resources::mount_mounts_path			( s_engine->get_mounts_path( ) );

	s_initialized						=	true;

	static resources::unmanaged_allocation_cook		s_unmanaged_allocation_cook;
	register_cook							( &s_unmanaged_allocation_cook );
}

void   vostok::core::finalize_resources	( )
{
	resources::finalize		( );
}

void   vostok::core::run_tests			( )
{
	testing::initialize			( s_engine );

	debug::notify_xbox_debugger ( "hello from core::initialize" );

	core_test_suite::singleton()->set_resources_path	( s_engine->get_resources_path() );
	core_test_suite::run_tests	( );
}

void vostok::core::finalize			( )
{
	R_ASSERT				( s_initialized, "core library hasn't been initialized" );

	tasks::finalize			( );
	testing::finalize		( );

#ifndef	MASTER_GOLD
	core::configs::finalize	( );
#endif	//MASTER_GOLD

	strings::finalize		( );
//	build::finalize			( );
//	timing::finalize		( );
	threading::finalize		( );
	memory::finalize		( );

	VOSTOK_DESTROY_REFERENCE	( s_core_synchronous_device );

	debug::finalize			( );

	s_initialized			= false;

	if ( testing::run_tests_command_line( ) )
	{
		fixed_string512		message;
		message.assignf		("program exit code: %d", s_engine->get_exit_code());
		debug::notify_xbox_debugger	(message.c_str());
	}
}

vostok::fs_new::synchronous_device_interface &	vostok::core::get_core_synchronous_device	( )
{
	return									* s_core_synchronous_device.c_ptr( );
}

void vostok::core::core_engine_flush( )
{
	if ( s_engine )
		s_engine->on_crash		( );
}

pcstr vostok::core::user_data_directory( )
{
	return									s_engine->get_user_data_directory( );
}
