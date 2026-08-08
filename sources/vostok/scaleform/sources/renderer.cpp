////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "flash_renderer.h"
#include "flash_movie.h"
#include "flash_text_manager.h"
#include "scaleform_render_command_queue.h"
#include "scaleform_render_command_queue_impl.h"

#include "GFx.h"
#include "GFx_Renderer_D3D1x.h"

namespace survarium {

void flash_renderer::present( flash_movie** movies, u32 movies_count, flash_text_manager* text_manager )
{
	for ( u32 i = 0; i < movies_count; ++i )
	{
		flash_movie* movie = movies[ i ];
		if ( movie->m_output_width != m_output_width || movie->m_output_height != m_output_height )
			movie->SetViewport( m_output_width, m_output_height );

		Scaleform::Render::TreeRootDisplayHandle& handle = *movie->m_handle;
		m_R2dRenderer->BeginFrame( );
		if ( handle.NextCapture( m_R2dRenderer->GetContextNotify( ) ) )
			m_R2dRenderer->Display( handle );
		m_R2dRenderer->EndFrame( );
	}

	if ( text_manager )
	{
		if (
			text_manager->m_output_width != m_output_width ||
			text_manager->m_output_height != m_output_height
		)
			text_manager->set_viewport( m_output_width, m_output_height );

		Scaleform::Render::TreeRootDisplayHandle handle =
			text_manager->text_manager_impl->GetDisplayHandle( );
		m_R2dRenderer->BeginFrame( );
		if ( handle.NextCapture( m_R2dRenderer->GetContextNotify( ) ) )
			m_R2dRenderer->Display( handle );
		m_R2dRenderer->EndFrame( );
	}
}

void flash_renderer::on_reset_device( u32 width, u32 height, ID3D11Device* device, ID3D11DeviceContext* context )
{
	m_output_width		= width;
	m_output_height		= height;

	if ( !m_HALRenderer->IsInitialized( ) )
		m_HALRenderer->InitHAL( Scaleform::Render::D3D1x::HALInitParams( device, context ) );
	else
		m_HALRenderer->RestoreAfterReset( );
}

flash_renderer::flash_renderer(
		scaleform_render_command_queue*	queue,
		ID3D11Device*					device,
		ID3D11DeviceContext*			context
	)
{
	m_HALRenderer = SF_NEW Scaleform::Render::D3D1x::HAL( queue->impl );
	m_R2dRenderer = SF_NEW Scaleform::Render::Renderer2D( m_HALRenderer );

	if ( !m_HALRenderer->IsInitialized( ) )
	{
		Scaleform::Render::D3D1x::HALInitParams params(
			device,
			context,
			0,
			reinterpret_cast< Scaleform::ThreadId >( ::GetCurrentThreadId( ) )
		);
		m_HALRenderer->InitHAL( params );
	}

	queue->impl->pHAL				= m_HALRenderer;
	queue->impl->pHALTextureMgr		= m_HALRenderer->GetTextureManager( );
	queue->impl->pR2D				= m_R2dRenderer;
	queue->impl->pRenderThreadId		= Scaleform::GetCurrentThreadId( );
}

} // namespace survarium
