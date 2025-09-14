////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "survarium_pc_application.h"
#include <vostok/engine/api.h>

using survarium::application;

void application::initialize( )
{
	m_exit_code						= 0;

	preinitialize					( );
	vostok::engine::initialize		( );
	postinitialize					( );
}

void application::finalize	( )
{
	vostok::engine::finalize			( );
}

void application::execute	( )
{
	vostok::engine::execute			( );
	m_exit_code						= vostok::engine::get_exit_code();
}