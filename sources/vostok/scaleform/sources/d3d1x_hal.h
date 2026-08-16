////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef D3D1X_HAL_H_INCLUDED
#define D3D1X_HAL_H_INCLUDED

// claude@NOTE: nothing #includes this header - it is a PDB-derived reference skeleton,
// listed in scaleform.vcproj but never compiled. Every symbol it describes is emitted
// (and already byte-exact) from the vendored SDK header it mirrors under
// scaleform/src/render/d3d1x/, so its STATE[STUB] bodies are documentation, not a
// matching gap - check the vendored header before treating one as work.
// survarium's fork of the Scaleform 4.x D3D1x_HAL.h - the shipped binary
// tracks a NEWER SDK than the vendored 4.0.15 (ShaderHAL base, filters,
// render events, MatrixState, ...); skeleton rebuilt from the canonical PDB
// type dumps. Per the rich index this fork header also CARRIED definitions of
// several base-SDK members (Render::HAL::~HAL and friends, at the bottom) -
// the fork backported the newer D3D11 backend onto an older core.

#include "Render/Render_ShaderHAL.h"
#include "d3d1x_meshcache.h"
#include "d3d1x_shader.h"
#include "d3d1x_texture.h"

#include <d3d11.h>

namespace Scaleform {
namespace Render {
namespace D3D1x {

struct HALInitParams : public Render::HALInitParams {
	inline						HALInitParams		(
									ID3D11Device*			arg_0,
									ID3D11DeviceContext*	arg_1,
									unsigned long			arg_2,
									void*					arg_3
								) { /* no source */ }

	inline	void				SetTextureManager	( TextureManager* arg_0 ) { /* no source */ }

	inline	TextureManager*		GetTextureManager	( ) const { /* no source */ return 0; }

	inline						~HALInitParams		( ) { /* no source */ }

public:
	/* 0x0000 */	/* Scaleform::Render::HALInitParams */
	/* 0x0018 */	ID3D11Device*			pD3DDevice;
	/* 0x001c */	ID3D11DeviceContext*	pD3DContext;
}; // struct HALInitParams

class RenderTargetData : public RenderBuffer::RenderTargetData {
public:
	virtual					~RenderTargetData	( ) { /* no source */ }

	// STATE[STUB]
	inline					RenderTargetData	(
								RenderBuffer*			arg_0,
								ID3D11View*				arg_1,
								DepthStencilBuffer*		arg_2,
								ID3D11View*				arg_3
							)
	{
		// FUNCTION BODY[0x0aee90]
		VOSTOK_UNREFERENCED_PARAMETERS	( arg_0, arg_1, arg_2, arg_3 );
	}

	// STATE[STUB]
	static	void			UpdateData			(
								RenderBuffer*			arg_0,
								ID3D11View*				arg_1,
								DepthStencilBuffer*		arg_2,
								ID3D11View*				arg_3
							)
	{
		// FUNCTION BODY[0x0aedd0]
		VOSTOK_UNREFERENCED_PARAMETERS	( arg_0, arg_1, arg_2, arg_3 );
	}

private:
	/* 0x0000 */	/* Scaleform::Render::RenderBuffer::RenderTargetData */
	/* 0x0010 */	ID3D11View*		pRenderSurface;
	/* 0x0014 */	ID3D11View*		pDSSurface;
}; // class RenderTargetData

// no addressed code (referenced-only); the SDK's D3D11 PIX event wrapper
class RenderEvent : public Render::RenderEvent {
public:
	virtual	void	Begin		( String arg_0 ) override { /* no source */ }

	virtual	void	End			( ) override { /* no source */ }

	inline			RenderEvent	( ) { /* no source */ }
	virtual			~RenderEvent( ) { /* no source */ }

	static	inline	void	InitializeEvents( ID3D11DeviceContext* arg_0 ) { /* no source */ }
	static	inline	void	ShutdownEvents	( ) { /* no source */ }
}; // class RenderEvent

class HAL : public Render::ShaderHAL< ShaderManager, ShaderInterface > {
public:
	enum RasterModes
	{
		RasterMode_Default		= 0x0,
		RasterMode_Wireframe	= 0x1,
		RasterMode_Count		= 0x2,
	};

	// everything unmarked is out-of-line in d3d1x_hal.cpp per the rich index
	inline	HAL*			getThis							( ) { /* no source */ return this; }

			explicit		HAL								( ThreadCommandQueue* arg_0 );
	virtual					~HAL							( );

	virtual	bool			InitHAL							( D3D1x::HALInitParams const& arg_0 );

	virtual	bool			ShutdownHAL						( );

	inline	void			PrepareForReset					( ) { /* no source */ }

			bool			RestoreAfterReset				( );

	virtual	bool			BeginScene						( ) override;

	virtual	bool			EndScene						( ) override;

	virtual	void			beginDisplay					( BeginDisplayData* arg_0 ) override;

	virtual	void			updateViewport					( ) override;

	virtual	PrimitiveFill*	CreatePrimitiveFill				( PrimitiveFillData const& arg_0 ) override;

	virtual	void			DrawProcessedPrimitive			(
								Primitive*			arg_0,
								PrimitiveBatch*		arg_1,
								PrimitiveBatch*		arg_2
							) override;

	virtual	void			DrawProcessedComplexMeshes		(
								ComplexMesh*		arg_0,
								StrideArray< MatrixPoolImpl::HMatrix > const&	arg_1
							) override;

	virtual	void			PushMask_BeginSubmit			( MaskPrimitive* arg_0 ) override;

	virtual	void			EndMaskSubmit					( ) override;

	virtual	void			PopMask							( ) override;

	// STATE[STUB]
	virtual	void			beginMaskDisplay				( ) override
	{
		// FUNCTION BODY[0x0aed60]
	}

			bool			checkMaskBufferCaps				( );

	virtual	void			clearSolidRectangle				( Rect< int > const& arg_0, Color arg_1 ) override;

	virtual	void			applyBlendModeImpl				( BlendMode arg_0, bool arg_1, bool arg_2 ) override;

			bool			createBlendStates				( );

			void			destroyBlendStates				( );

			bool			createDepthStencilStates		( );

			void			destroyDepthStencilStates		( );

	inline	void			SetRasterMode					( RasterModes arg_0 ) { /* no source */ }

			bool			createRasterStates				( );

			void			destroyRasterStates				( );

	inline	ID3D11Device*	GetDevice						( ) const { /* no source */ return pDevice; }

			bool			createConstantBuffers			( );

			void			destroyConstantBuffers			( );

			ID3D11Buffer*	getNextConstantBuffer			( );

	// STATE[STUB]
	virtual	Render::TextureManager*	GetTextureManager		( ) const override
	{
		// FUNCTION BODY[0x0aed80]
		return pTextureManager;
	}

	virtual	RenderTarget*	CreateRenderTarget				( ID3D11View* arg_0, ID3D11View* arg_1 );
	virtual	RenderTarget*	CreateRenderTarget				( Render::Texture* arg_0, bool arg_1 ) override;

	virtual	RenderTarget*	CreateTempRenderTarget			( Size< unsigned long > const& arg_0, bool arg_1 ) override;

	virtual	bool			SetRenderTarget					( RenderTarget* arg_0, bool arg_1 ) override;

	virtual	void			PushRenderTarget				(
								Rect< float > const&	arg_0,
								RenderTarget*			arg_1,
								u32						arg_2
							) override;

	virtual	void			PopRenderTarget					( u32 arg_0 ) override;

	virtual	bool			createDefaultRenderBuffer		( ) override;

	virtual	void			PushFilters						( FilterPrimitive* arg_0 ) override;

	virtual	void			drawUncachedFilter				( FilterStackEntry const& arg_0 ) override;

	virtual	void			drawCachedFilter				( FilterPrimitive* arg_0 ) override;

	// STATE[STUB]
	virtual	D3D1x::MeshCache&	GetMeshCache				( ) override
	{
		// FUNCTION BODY[0x0aed90]
		return Cache;
	}

	// STATE[STUB]
	virtual	void			MapVertexFormat					(
								PrimitiveFillType		arg_0,
								VertexFormat const*		arg_1,
								VertexFormat const**	arg_2,
								VertexFormat const**	arg_3,
								VertexFormat const**	arg_4,
								u32						arg_5
							) override
	{
		// FUNCTION BODY[0x0aeda0]
		VOSTOK_UNREFERENCED_PARAMETERS	( arg_0, arg_1, arg_2, arg_3, arg_4, arg_5 );
	}

	virtual	void			setBatchUnitSquareVertexStream	( ) override;

	virtual	void			drawPrimitive					( u32 arg_0, u32 arg_1 ) override;

			void			drawIndexedPrimitive			(
								u32		arg_0,
								u32		arg_1,
								u32		arg_2,
								u32		arg_3
							);

			void			drawIndexedInstanced			(
								u32		arg_0,
								u32		arg_1,
								u32		arg_2,
								u32		arg_3
							);

	virtual	bool			shouldRenderFilters				( FilterPrimitive const* arg_0 ) const override;

	virtual	Render::RenderEvent&	GetEvent				( EventType arg_0 ) override;

	virtual	void			drawScreenQuad					( ) override;

	static	inline	u32		GetBlendType					(
								BlendMode		arg_0,
								ColorWriteMode	arg_1,
								bool			arg_2
							) { /* no source */ return 0; }

private:
	/* 0x0000 */	/* Scaleform::Render::ShaderHAL< ShaderManager, ShaderInterface > */
	/* 0xf930 */	ID3D11Device*				pDevice;
	/* 0xf934 */	ID3D11DeviceContext*		pDeviceContext;
	/* 0xf938 */	Ptr< ID3D11RenderTargetView >	pRenderTargetView;
	/* 0xf93c */	Ptr< ID3D11DepthStencilView >	pDepthStencilView;
	/* 0xf940 */	MeshCache					Cache;
	/* 0xfac8 */	Ptr< TextureManager >		pTextureManager;
	/* 0xfacc */	PrimitiveBatch::BatchType	PrevBatchType;
	/* 0xfad0 */	bool						StencilChecked;
	/* 0xfad1 */	bool						StencilAvailable;
	/* 0xfad2 */	bool						DepthBufferAvailable;
	/* 0xfad4 */	ID3D11BlendState*			BlendStates[37];
	/* 0xfb68 */	ID3D11DepthStencilState*	DepthStencilStates[8];
	/* 0xfb88 */	RasterModes					RasterMode;
	/* 0xfb8c */	ID3D11RasterizerState*		RasterStates[2];
	/* 0xfb94 */	ID3D11Buffer*				ConstantBuffers[8];
	/* 0xfbb4 */	u32							CurrentConstantBuffer;
}; // class HAL

} // namespace D3D1x

// base-SDK member bodies the fork header carried (rich index attributes their
// line info to vostok/scaleform/sources/d3d1x_hal.h) - the fork backported
// the D3D11 backend, supplying these out of line here:
//
//	// FUNCTION BODY[0x0aef80]	Scaleform::Render::HAL::~HAL()
//	// FUNCTION BODY[0x0af090]	Scaleform::Render::D3D1x::ShaderManager::~ShaderManager()
//	// FUNCTION BODY[0x0af130]	Scaleform::Render::HAL::RenderTargetEntry::RenderTargetEntry()
//	// FUNCTION BODY[0x0af1b0]	Scaleform::Render::HAL::RenderTargetEntry::operator=(RenderTargetEntry const&)
//	// FUNCTION BODY[0x0af280]	Scaleform::Render::MatrixState::operator=(MatrixState const&)
//	// FUNCTION BODY[0x0af580]	Scaleform::Render::HAL::FilterStackEntry::~FilterStackEntry()
//
// They need the (newer-SDK) base class definitions, so they stay recorded
// here as carcass markers until that base is reconciled (TU enablement pass).

} // namespace Render
} // namespace Scaleform

STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::HALInitParams, 0x20);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::RenderTargetData, 0x18);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::RenderEvent, 0x4);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::HAL, 0xFBB8);

#endif // #ifndef D3D1X_HAL_H_INCLUDED
