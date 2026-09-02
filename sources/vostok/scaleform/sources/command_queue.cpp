// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "scaleform_game_engine.h"
#include "scaleform_render_command.h"
#include "scaleform_render_command_queue.h"
#include "scaleform_render_command_queue_impl.h"

namespace survarium {

scaleform_render_command_queue::~scaleform_render_command_queue( )
{
	delete impl;
}

void scaleform_render_command_queue_impl::PushThreadCommand( Scaleform::Render::ThreadCommand* command )
{
	scaleform_render_command	cmd	= { command };
	engine.execute_scaleform_command( cmd );
}

void scaleform_render_command_queue_impl::GetRenderInterfaces( Scaleform::Render::Interfaces* interfaces )
{
	interfaces->pHAL			= pHAL;
	interfaces->pRenderer2D		= pR2D;
	interfaces->pTextureManager	= pHALTextureMgr;
	interfaces->RenderThreadID	= pRenderThreadId;
}

void scaleform_render_command::execute( )
{
	thread_command->Execute( );
}

scaleform_render_command_queue::scaleform_render_command_queue( scaleform_game_engine& engine )
{
	impl	= new scaleform_render_command_queue_impl( engine );
}

} // namespace survarium
