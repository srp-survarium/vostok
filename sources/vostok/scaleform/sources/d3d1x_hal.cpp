////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "d3d1x_hal.h"

namespace Scaleform {
namespace Render {
namespace D3D1x {

// STATE[STUB]
void HAL::drawScreenQuad( )
{
	// FUNCTION BODY[0x5b6ac0]
}

// STATE[STUB]
void HAL::drawIndexedInstanced(
		unsigned int,
		unsigned int,
		unsigned int,
		unsigned int
	)
{
	// FUNCTION BODY[0x5b6b10]
}

// STATE[STUB]
void HAL::drawIndexedPrimitive(
		unsigned int,
		unsigned int,
		unsigned int,
		unsigned int
	)
{
	// FUNCTION BODY[0x5b6b40]
}

// STATE[STUB]
void HAL::drawPrimitive( unsigned int, unsigned int )
{
	// FUNCTION BODY[0x5b6b60]
}

// STATE[STUB]
void HAL::setBatchUnitSquareVertexStream( )
{
	// FUNCTION BODY[0x5b6b80]
}

// STATE[STUB]
ID3D11Buffer* HAL::getNextConstantBuffer( )
{
	// FUNCTION BODY[0x5b6bb0]
	return 0;
}

// STATE[STUB]
void HAL::destroyConstantBuffers( )
{
	// FUNCTION BODY[0x5b6bd0]
}

// STATE[STUB]
bool HAL::createConstantBuffers( )
{
	// FUNCTION BODY[0x5b6c20]
	return false;
}

// STATE[STUB]
void HAL::destroyRasterStates( )
{
	// FUNCTION BODY[0x5b6cc0]
}

// STATE[STUB]
bool HAL::createRasterStates( )
{
	// FUNCTION BODY[0x5b6d00]
	return false;
}

// STATE[STUB]
void HAL::destroyDepthStencilStates( )
{
	// FUNCTION BODY[0x5b6d90]
}

// STATE[STUB]
bool HAL::createDepthStencilStates( )
{
	// FUNCTION BODY[0x5b6de0]
	return false;
}

// STATE[STUB]
void HAL::destroyBlendStates( )
{
	// FUNCTION BODY[0x5b6f70]
}

// STATE[STUB]
bool HAL::createBlendStates( )
{
	// FUNCTION BODY[0x5b6fb0]
	return false;
}

// STATE[STUB]
bool HAL::checkMaskBufferCaps( )
{
	// FUNCTION BODY[0x5b70d0]
	return false;
}

// STATE[STUB]
bool HAL::shouldRenderFilters( Scaleform::Render::FilterPrimitive const* ) const
{
	// FUNCTION BODY[0x5b71e0]
	return false;
}

// STATE[STUB]
void HAL::updateViewport( )
{
	// FUNCTION BODY[0x5b7220]
}

// STATE[STUB]
bool HAL::RestoreAfterReset( )
{
	// FUNCTION BODY[0x5b7360]
	return false;
}

// STATE[STUB]
Scaleform::Render::RenderEvent& HAL::GetEvent( Scaleform::Render::EventType )
{
	// FUNCTION BODY[0x5b73e0]
	// the original keeps the SDK's `static RenderEvent defaultEvent` fallback
	// here (its atexit dtor is addressed in this TU)
	static Scaleform::Render::RenderEvent dummy;
	return dummy;
}

// STATE[STUB]
Scaleform::Render::RenderTarget* HAL::CreateTempRenderTarget( Scaleform::Render::Size<unsigned long> const&, bool )
{
	// FUNCTION BODY[0x5b7420]
	return 0;
}

// STATE[STUB]
Scaleform::Render::RenderTarget* HAL::CreateRenderTarget( Scaleform::Render::Texture*, bool )
{
	// FUNCTION BODY[0x5b7540]
	return 0;
}

// STATE[STUB]
Scaleform::Render::RenderTarget* HAL::CreateRenderTarget( ID3D11View*, ID3D11View* )
{
	// FUNCTION BODY[0x5b7670]
	return 0;
}

// STATE[STUB]
bool HAL::ShutdownHAL( )
{
	// FUNCTION BODY[0x5b77d0]
	return false;
}

// STATE[STUB]
bool HAL::createDefaultRenderBuffer( )
{
	// FUNCTION BODY[0x5b78d0]
	return false;
}

// STATE[STUB]
bool HAL::SetRenderTarget( Scaleform::Render::RenderTarget*, bool )
{
	// FUNCTION BODY[0x5b7b20]
	return false;
}

// STATE[STUB]
void HAL::drawCachedFilter( Scaleform::Render::FilterPrimitive* )
{
	// FUNCTION BODY[0x5b7c20]
}

// STATE[STUB]
void HAL::drawUncachedFilter( Scaleform::Render::HAL::FilterStackEntry const& )
{
	// FUNCTION BODY[0x5b81e0]
}

// STATE[STUB]
void HAL::PushFilters( Scaleform::Render::FilterPrimitive* )
{
	// FUNCTION BODY[0x5b8860]
}

// STATE[STUB]
void HAL::PopRenderTarget( unsigned int )
{
	// FUNCTION BODY[0x5b8b30]
}

// STATE[STUB]
void HAL::PushRenderTarget(
		Scaleform::Render::Rect<float> const&,
		Scaleform::Render::RenderTarget*,
		unsigned int
	)
{
	// FUNCTION BODY[0x5b8d30]
}

// STATE[STUB]
void HAL::applyBlendModeImpl(
		Scaleform::Render::BlendMode,
		bool,
		bool
	)
{
	// FUNCTION BODY[0x5b90e0]
}

// STATE[STUB]
void HAL::clearSolidRectangle(
		Scaleform::Render::Rect<int> const&,
		Scaleform::Render::Color
	)
{
	// FUNCTION BODY[0x5b91a0]
}

// STATE[STUB]
void HAL::PopMask( )
{
	// FUNCTION BODY[0x5b9310]
}

// STATE[STUB]
void HAL::EndMaskSubmit( )
{
	// FUNCTION BODY[0x5b9490]
}

// STATE[STUB]
void HAL::DrawProcessedComplexMeshes(
		Scaleform::Render::ComplexMesh*,
		Scaleform::Render::StrideArray<Scaleform::Render::MatrixPoolImpl::HMatrix> const&
	)
{
	// FUNCTION BODY[0x5b95a0]
}

// STATE[STUB]
Scaleform::Render::PrimitiveFill* HAL::CreatePrimitiveFill( Scaleform::Render::PrimitiveFillData const& )
{
	// FUNCTION BODY[0x5b9cb0]
	return 0;
}

// STATE[STUB]
void HAL::beginDisplay( Scaleform::Render::BeginDisplayData* )
{
	// FUNCTION BODY[0x5b9d00]
}

// STATE[STUB]
bool HAL::EndScene( )
{
	// FUNCTION BODY[0x5b9d60]
	return false;
}

// STATE[STUB]
bool HAL::BeginScene( )
{
	// FUNCTION BODY[0x5b9e10]
	return false;
}

// STATE[STUB]
HAL::~HAL( )
{
	// FUNCTION BODY[0x5b9ef0]
}

// STATE[STUB]
void HAL::PushMask_BeginSubmit( Scaleform::Render::MaskPrimitive* )
{
	// FUNCTION BODY[0x5b9f60]
}

// STATE[STUB]
void HAL::DrawProcessedPrimitive(
		Scaleform::Render::Primitive*,
		Scaleform::Render::PrimitiveBatch*,
		Scaleform::Render::PrimitiveBatch*
	)
{
	// FUNCTION BODY[0x5ba4c0]
}

// STATE[STUB]
HAL::HAL( Scaleform::Render::ThreadCommandQueue* )
{
	// FUNCTION BODY[0x5ba760]
}

// STATE[STUB]
bool HAL::InitHAL( Scaleform::Render::D3D1x::HALInitParams const& )
{
	// FUNCTION BODY[0x5ba830]
	return false;
}

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform
