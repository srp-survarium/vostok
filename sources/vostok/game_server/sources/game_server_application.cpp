////////////////////////////////////////////////////////////////////////////
//	Created		: 27.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_server_application.h"
#include "game_server_memory.h"
#include <vostok/core/core.h>
#include <vostok/core/simple_engine.h>
#include <vostok/os_include.h>		// for GetCommandLine
#include <vostok/network/api.h>
#include <vostok/network/engine.h>
#include <vostok/network/world.h>
#include <vostok/network/server.h>
#include <vostok/network/packet_reader.h>

using vostok::game_server::application;

vostok::memory::doug_lea_allocator_type	vostok::game_server::g_allocator;

typedef vostok::core::simple_engine							core_engine_type;
static vostok::uninitialized_reference< core_engine_type >	s_core_engine;

struct network_engine : vostok::network::engine {};

static void on_packet_received		( vostok::network::server& server, vostok::network::client_session& client, vostok::network::packet_reader& packet )
{
	string256 message;
	packet.r_string					( message );

	vostok::network::packet* const answer_packet = NEW (vostok::network::packet) ( vostok::game_server::g_allocator );
	answer_packet->append			( message );
	server.send_broadcast			( answer_packet );
}

void application::initialize		( )
{
	m_exit_code						= 0;

	VOSTOK_CONSTRUCT_REFERENCE		( s_core_engine, core_engine_type );

	pstr command_line				= 0;
	STR_JOINA						( command_line, GetCommandLine(), " -no_warning_on_page_file_size -max_resources_size=16" );

	core::preinitialize				(
		&*s_core_engine,
		logging::create_log,
		command_line, 
		command_line::contains_application_true,
		"game_server",
		__DATE__ 
	);

	g_allocator.do_register			( 4*Mb, "game_server" );

	memory::allocate_region			( );

	core::initialize				( "../../resources/sources", "game_server", core::perform_debug_initialization );

	static network_engine network_engine;
	vostok::network::memory_allocator	( g_allocator );
	m_network						= vostok::network::create_world( network_engine, g_allocator );
	m_network->initialize			( );

	m_server						= NEW( vostok::network::server ) ( *m_network );
	m_server->set_on_packet_received( boost::bind( &on_packet_received, boost::ref(*m_server), _1, _2) );
	m_server->start_accepting		( 2510, 16 );
}

void application::finalize			( )
{
	m_network->finalize				( );
	VOSTOK_DELETE_IMPL				( g_allocator, m_server );
	vostok::network::destroy_world	( m_network );

	core::finalize					( );

	VOSTOK_DESTROY_REFERENCE			( s_core_engine );
}

void application::execute			( )
{
	for (;;) {
		m_network->dispatch_callbacks ( );
		m_network->tick				( );
	}

	//m_exit_code						= s_core_engine->get_exit_code();
}