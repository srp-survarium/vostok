////////////////////////////////////////////////////////////////////////////
//	Created		: 13.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "rtp_learn_application.h"
#include "rtp_learn_memory.h"
#include <vostok/core/core.h>
#include <vostok/core/simple_engine.h>
#include <vostok/os_include.h>		// for GetCommandLine
#include <vostok/rtp/api.h>
#include <vostok/animation/api.h>
#include <vostok/animation/engine.h>
#include <vostok/rtp/engine.h>
#include <vostok/rtp/base_controller.h>
#include <vostok/resources_fs.h>
#include <vostok/game_test_suite.h>
#include <vostok/fs/simple_synchronous_interface.h>

vostok::memory::doug_lea_allocator_type	vostok::rtp_learn::g_allocator;

namespace vostok {
namespace rtp_learn {



//command_line::key	cmd_test("rtp_test", "", "rtp_learn", "");
//command_line::key	cmd_save_test( "rtp_save_test", "", "rtp", "" );

class rtp_learn_engine :
	public vostok::core::simple_engine,
	public vostok::animation::engine,
	public vostok::rtp::engine
{

public:
								rtp_learn_engine		( );
			void				initialize				( );
			void				finalize				( );

private:
	virtual	pcstr				get_mounts_path			( ) const { return "../../mounts"; }

public:
	virtual	rtp::world&			get_rtp_world			( ) { return *m_rtp_world; }
			
private:
	rtp::world*					m_rtp_world;
	animation::world*			m_animation_world;

}; // class rtp_learn_engine


rtp_learn_engine::rtp_learn_engine				( ) :
	m_animation_world			( 0 ),
	m_rtp_world					( 0 )
{
}

void	rtp_learn_engine::initialize			( )
{
	ASSERT						( !m_animation_world );
	m_animation_world			= animation::create_world( *this, 0, false );

	ASSERT						( !m_rtp_world );
	m_rtp_world					= rtp::create_world( *this, *m_animation_world, 0, get_resources_path() );
}


typedef vostok::rtp_learn::rtp_learn_engine						rtp_learn_engine_type;
static vostok::uninitialized_reference< rtp_learn_engine_type >	s_rtp_learn_engine;

void	rtp_learn_engine::finalize				( )
{

	
	rtp::destroy_world( m_rtp_world );
	animation::destroy_world( m_animation_world );
}



application::application		( ):
m_controller( 0 )
{												

}

vostok::uninitialized_reference< vostok::fs_new::simple_synchronous_interface >	s_fs_devices;
static vostok::resources::mount_ptr	s_resources_mount;

static void on_resources_mounted		( vostok::resources::mount_ptr result )
{
	CHECK_OR_EXIT						( result, "Cannot mount resources" );
	s_resources_mount					= result;
}

bool	application::is_loaded( )
{
	R_ASSERT( m_controller );
	
	return m_controller->is_loaded();
}

void application::initialize( )
{
	m_exit_code				= 0;

	VOSTOK_CONSTRUCT_REFERENCE( s_rtp_learn_engine, rtp_learn_engine_type );

	pstr new_command_line	= 0;
	STR_JOINA				( new_command_line, GetCommandLine(), " ", "-rtp_learn_singlethreaded", " ", "-rtp_test" );

	core::preinitialize		(
		& * s_rtp_learn_engine,
		logging::create_log, 
		new_command_line, 
		command_line::contains_application_true,
		"rtp_learn",
		__DATE__ 
	);

	g_allocator.do_register				( 8 * Kb, "rtp_learn" );
	
	m_animation_allocator.do_register	(  2*Mb,	"animation"	);
	m_rtp_allocator.do_register			(  8*Mb,	"rtp"	);

	memory::allocate_region	( );

	animation::set_memory_allocator	( m_animation_allocator );
	//rtp::set_memory_allocator		( memory::g_mt_allocator );//m_rtp_allocator );
	rtp::set_memory_allocator		( m_rtp_allocator );//m_rtp_allocator );

	core::initialize		( "../../resources/sources/", "rtp_learn", core::perform_debug_initialization );

	VOSTOK_CONSTRUCT_REFERENCE	( s_fs_devices, vostok::fs_new::simple_synchronous_interface ) 
									( vostok::fs_new::watcher_enabled_true );

	vostok::core::initialize_resources	( s_fs_devices->hdd_async, s_fs_devices->dvd_async, resources::enable_fs_watcher_false );
	vostok::resources::start_resources_threads		( );
	
 	resources::query_mount	( "resources", & on_resources_mounted, &g_allocator );

	//resources::query_mount	( "resources/tests", on_mounted_disk, &g_allocator );

	m_rtp_allocator.user_current_thread_id			( );
	m_animation_allocator.user_current_thread_id	( );
	s_rtp_learn_engine->initialize();


	pcstr controller_set_path = "resources/tests/rtp/navgation_controller/animation/controller_set.lua";
	m_controller = s_rtp_learn_engine->get_rtp_world().create_navigation_controller( "resources/animations/controller_set.lua", controller_set_path );
	//m_controller = s_rtp_learn_engine->get_rtp_world().create_navigation_controller( "resources/tests/rtp/navgation_controller/animation/controller_set.lua" );
	

	for ( ;; ) {
		vostok::resources::dispatch_callbacks	( );
		vostok::threading::yield	( 10 );
		
		if( is_loaded() )
			break;

	}
	
}

void application::finalize	( )
{
	s_rtp_learn_engine->get_rtp_world().destroy_controller( m_controller );
	
	s_rtp_learn_engine->finalize( );
	vostok::resources::dispatch_callbacks	( );
 	
	s_resources_mount		= NULL;

 	resources::wait_and_dispatch_callbacks ( true );

	resources::finish_resources_threads	( );
	core::finalize_resources( );
	core::finalize			( );

	VOSTOK_DESTROY_REFERENCE	( s_fs_devices );
	VOSTOK_DESTROY_REFERENCE	( s_rtp_learn_engine );
}

static vostok::command_line::key s_use_tasks	("use_tasks", "", "", "use tasks system in learning process");

void application::execute	( )
{
	//R_ASSERT( s_rtp_learn_engine->get_rtp_world() );
	
	game_test_suite::singleton()->set_rtp_world( &s_rtp_learn_engine->get_rtp_world() );
	game_test_suite::run_tests	( );
	

	vostok::timing::timer			timer;

	timer.start					( );

	do {
		vostok::resources::dispatch_callbacks	( );
	} while (
		m_controller->learn_step( 70.f, 100 )
		//s_rtp_learn_engine->get_rtp_world().learn_step( 70.f )//, s_use_tasks.is_set() )
	);

	LOG_INFO				(
		logging::format_specifier_message,
		logging::log_to_console,
		"elapsed %5.2fs", timer.get_elapsed_sec()
	);
	
	m_exit_code				= s_rtp_learn_engine->get_exit_code();
}



} // namespace rtp_learn
} // namespace vostok 