////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "receiver_keyboard.h"
#include "direct_input_include.h"
#include <vostok/input/handler.h>

using vostok::input::receiver::keyboard;
using vostok::input::handler;
using vostok::input::enum_keyboard;
using vostok::input::world;

keyboard::keyboard			( IDirectInput8A& direct_input, HWND const window_handle, world& input_world ) :
	m_current_events_count	( 0 ),
	m_window_handle		( window_handle ),
	m_device			( 0 ),
	m_world				( input_world )
{
	HRESULT	result		= direct_input.CreateDevice(GUID_SysKeyboard, &m_device, 0);
	R_ASSERT			( !FAILED( result ) );

	result				= m_device->SetDataFormat( &c_dfDIKeyboard );
	R_ASSERT			( !FAILED( result ) );

    result				=
		m_device->SetCooperativeLevel(
			window_handle,
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
		);
	R_ASSERT			( !FAILED( result ) );

	DIPROPDWORD			dipdw;
	dipdw.diph.dwSize		= sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize	= sizeof( DIPROPHEADER );
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= 256;
	m_device->SetProperty	( DIPROP_BUFFERSIZE, &dipdw.diph );

	memory::zero		( &m_current_key_state, sizeof( m_current_key_state ) );
}

keyboard::~keyboard			( )
{
	m_device->Unacquire	( );
	m_device->Release	( );
}

void keyboard::reset_current_state	( )
{
	memory::zero		( &m_current_key_state, sizeof( m_current_key_state ) );
}

void keyboard::on_activate	( )
{
	HRESULT	result		= m_device->Acquire	( );
	if ( FAILED( result ) )
		LOG_ERROR		( "KeyboardDevice Acquire FAILED" );

	reset_current_state	( );
}

void keyboard::on_deactivate( )
{
	m_device->Unacquire	( );
}

bool keyboard::is_key_down	( char const value ) const
{
	return				( ( (u32)value >> 7 ) & 1 );
}

bool keyboard::is_key_down	( enum_keyboard const key ) const
{
	return				( is_key_down( (char)m_current_key_state[ key ] ) );
}

// claude@NOTE: structure is the faithful flat-if shape (5 sequential error checks +
// reacquire + fill loop); residual is the non-steerable LOG-machinery wall - the target
// builds the log-callback boost::function INLINE (stored_vtable + g_log_callback,
// boost::function1::clear) while our build emits a boost::function<>::function<> ctor CALL
// + ~function (same wall caps mouse::execute at 75% with ONE LOG; this has FIVE).
// See docs/binary_matching/patterns/log-callback-ctor-schedule.md.
void keyboard::execute		( )
{
	unsigned long		events_count = sizeof( m_current_events ) / sizeof( m_current_events[ 0 ] );
	HRESULT	result		= m_device->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), m_current_events, &events_count, 0 );
	if ( FAILED( result ) ) {
		if ( result == DIERR_INPUTLOST )
			LOG_ERROR		( "DIERR_INPUTLOST" );

		if ( result == DIERR_INVALIDPARAM ) {
			LOG_ERROR		( "DIERR_INVALIDPARAM" );
			return;
		}

		if ( result == DIERR_NOTINITIALIZED ) {
			LOG_ERROR		( "DIERR_NOTINITIALIZED" );
			return;
		}

		if ( result == E_PENDING ) {
			LOG_ERROR		( "E_PENDING" );
			return;
		}
	}

	if ( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED ) {
		result			= m_device->Acquire( );
		reset_current_state	( );
		if ( FAILED( result ) )
			return;

		events_count	= sizeof( m_current_events ) / sizeof( m_current_events[ 0 ] );
		result			= m_device->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), m_current_events, &events_count, 0 );
		if ( FAILED( result ) ) {
			LOG_ERROR	( "can't get mouse keyboard" );
			return;
		}
	}

	if ( FAILED( result ) )
		return;

	m_current_events_count	= events_count;
	for ( u32 i = 0; i < m_current_events_count; ++i )
		m_current_key_state[ m_current_events[ i ].dwOfs ] = m_current_events[ i ].dwData & 0x80;
}

