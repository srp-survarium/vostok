////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef D3D1X_SYNC_H_INCLUDED
#define D3D1X_SYNC_H_INCLUDED

// survarium's fork of the Scaleform 4.x D3D1x_Sync.h (header-only; every
// addressed RenderSync body lives HERE per the rich index). The vendored
// 4.0.15 SDK predates the RenderSync/fence machinery entirely - the base
// Scaleform::Render::RenderSync comes from the newer SDK the fork tracked.

#include "Render/Render_Sync.h"

#include <d3d11.h>

namespace Scaleform {
namespace Render {
namespace D3D1x {

class RenderSync : public Render::RenderSync {
public:
	inline			RenderSync		( ) { /* no source */ }

	// STATE[STUB]
	inline	bool	SetDevice		( ID3D11Device* arg_0, ID3D11DeviceContext* arg_1 )
	{
		// FUNCTION BODY[0x0b4600]
		VOSTOK_UNREFERENCED_PARAMETERS	( arg_0, arg_1 );
		return false;
	}

	// STATE[STUB]
	virtual	void	BeginFrame		( ) override
	{
		// FUNCTION BODY[0x0b46d0]
	}

	// STATE[STUB]
	virtual	bool	EndFrame		( ) override
	{
		// FUNCTION BODY[0x0b4720]
		return false;
	}

	// STATE[STUB]
	virtual	void	KickOffFences	( FenceType arg_0 ) override
	{
		// FUNCTION BODY[0x022c50] (ICF-folded empty body; the fold also
		// absorbs Render::MeshCache::PostUpdateMesh)
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
	}

	// STATE[STUB]
	virtual	u64		SetFence		( ) override
	{
		// FUNCTION BODY[0x0b4770]
		return 0;
	}

	// STATE[STUB]
	virtual	bool	IsPending		( FenceType arg_0, u64 arg_1, FenceFrame const& arg_2 ) override
	{
		// FUNCTION BODY[0x0b4790]
		VOSTOK_UNREFERENCED_PARAMETERS	( arg_0, arg_1, arg_2 );
		return false;
	}

	// STATE[STUB]
	virtual	void	WaitFence		( FenceType arg_0, u64 arg_1, FenceFrame const& arg_2 ) override
	{
		// FUNCTION BODY[0x0b47d0]
		VOSTOK_UNREFERENCED_PARAMETERS	( arg_0, arg_1, arg_2 );
	}

	// STATE[STUB]
	virtual	void	ReleaseFence	( u64 arg_0 ) override
	{
		// FUNCTION BODY[0x0b4870]
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
	}

	virtual			~RenderSync		( ) { /* no source */ }

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

#endif // #ifndef D3D1X_SYNC_H_INCLUDED
