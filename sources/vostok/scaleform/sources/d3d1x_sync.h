////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef INC_SF_D3D1x_Sync_H
#define INC_SF_D3D1x_Sync_H

#include "Render/Render_Sync.h"
#include "Kernel/SF_Threads.h"

#include <d3d11.h>

namespace Scaleform {
namespace Render {
namespace D3D1x {

class RenderSync : public Render::RenderSync {
public:
	inline			RenderSync		( )
		: pNextEndFrameFence( NULL ),
		  pDevice( NULL ),
		  pDeviceContext( NULL )
	{
	}

	inline	bool	SetDevice		( ID3D11Device* device, ID3D11DeviceContext* device_context )
	{
		if ( !device || !device_context )
		{
			pDevice			= NULL;
			pDeviceContext	= NULL;
			ReleaseOutstandingFrames( );
			return true;
		}

		D3D11_QUERY_DESC desc;
		memset( &desc, 0, sizeof( desc ) );
		desc.Query	= D3D11_QUERY_EVENT;

		ID3D11Query* query	= NULL;
		HRESULT const result	= device->CreateQuery( &desc, &query );
		if ( query )
		{
			query->Release( );
			pDevice			= device;
			pDeviceContext	= device_context;
		}

		return SUCCEEDED( result );
	}

	virtual	void	BeginFrame		( ) override
	{
		if ( !pDevice )
			return;

		SF_ASSERT( !pNextEndFrameFence );

		D3D11_QUERY_DESC desc;
		memset( &desc, 0, sizeof( desc ) );
		desc.Query	= D3D11_QUERY_EVENT;

		HRESULT const result	= pDevice->CreateQuery( &desc, &pNextEndFrameFence );
		if ( FAILED( result ) )
			pNextEndFrameFence	= NULL;

		Render::RenderSync::BeginFrame( );
	}

	virtual	bool	EndFrame		( ) override
	{
		if ( !pDevice || !pDeviceContext )
			return true;

		if ( !Render::RenderSync::EndFrame( ) )
			return false;

		if ( pNextEndFrameFence )
		{
			pDeviceContext->End( pNextEndFrameFence );
			pNextEndFrameFence->Release( );
		}

		pNextEndFrameFence	= NULL;
		return true;
	}

	virtual	void	KickOffFences	( FenceType wait_type ) override
	{
		SF_UNUSED( wait_type );
	}

protected:
	virtual	u64		SetFence		( ) override
	{
		if ( pNextEndFrameFence )
			pNextEndFrameFence->AddRef( );

		return reinterpret_cast< u64 >( pNextEndFrameFence );
	}

	virtual	bool	IsPending		( FenceType wait_type, u64 handle, FenceFrame const& parent ) override
	{
		SF_UNUSED2( wait_type, parent );

		ID3D11Query* query	= reinterpret_cast< ID3D11Query* >( handle );
		if ( !query )
			return false;

		if ( FenceFrames.IsEmpty( ) )
			return false;

		if ( query == pNextEndFrameFence )
			return true;

		HRESULT const result	= pDeviceContext->GetData(
			query,
			NULL,
			0,
			D3D11_ASYNC_GETDATA_DONOTFLUSH
		);
		return result != S_OK;
	}

	virtual	void	WaitFence		( FenceType wait_type, u64 handle, FenceFrame const& parent ) override
	{
		SF_UNUSED2( wait_type, parent );

		ID3D11Query* query	= reinterpret_cast< ID3D11Query* >( handle );
		if ( !query )
			return;

		if ( query == pNextEndFrameFence )
		{
			pDeviceContext->End( pNextEndFrameFence );
			query	= pNextEndFrameFence;

			D3D11_QUERY_DESC desc;
			memset( &desc, 0, sizeof( desc ) );
			desc.Query	= D3D11_QUERY_EVENT;

			HRESULT const result	= pDevice->CreateQuery( &desc, &pNextEndFrameFence );
			if ( FAILED( result ) )
				pNextEndFrameFence	= NULL;
		}

		while ( pDeviceContext->GetData( query, NULL, 0, 0 ) != S_OK )
			Thread::Sleep( 0 );
	}

	virtual	void	ReleaseFence	( u64 handle ) override
	{
		ID3D11Query* query	= reinterpret_cast< ID3D11Query* >( handle );
		if ( query )
			query->Release( );
	}

private:
	/* 0x0000 */	/* Scaleform::Render::RenderSync */
	/* 0x0060 */	ID3D11Query*				pNextEndFrameFence;
	/* 0x0064 */	Ptr< ID3D11Device >			pDevice;
	/* 0x0068 */	Ptr< ID3D11DeviceContext >	pDeviceContext;
}; // class RenderSync

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform

STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::RenderSync, 0x70);

#endif // #ifndef INC_SF_D3D1x_Sync_H
