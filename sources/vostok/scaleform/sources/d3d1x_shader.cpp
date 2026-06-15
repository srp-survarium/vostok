////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "d3d1x_shader.h"
#include "d3d1x_hal.h"

namespace Scaleform {
namespace Render {
namespace D3D1x {

// STATE[STUB]
void ShaderConstantRange::Update(
		int,
		int,
		int
	)
{
	// FUNCTION BODY[0x0b3790]
}

// STATE[STUB]
void ShaderManager::EndScene( )
{
	// FUNCTION BODY[0x5b6170]
}

// STATE[STUB]
void ShaderManager::BeginScene( )
{
	// FUNCTION BODY[0x5b6180]
}

// STATE[STUB]
bool ShaderManager::HasInstancingSupport( ) const
{
	// FUNCTION BODY[0x5b6190]
	return false;
}

// STATE[STUB]
void ShaderInterface::BeginScene( )
{
	// FUNCTION BODY[0x5b61a0]
}

// STATE[STUB]
void ShaderInterface::SetTexture(
		const Scaleform::Render::D3D1x::ShaderPair,
		unsigned int,
		Scaleform::Render::Texture*,
		Scaleform::Render::ImageFillMode,
		unsigned int
	)
{
	// FUNCTION BODY[0x5b6210]
}

// STATE[STUB]
void FragShader::Shutdown( )
{
	// FUNCTION BODY[0x5b6240]
}

// STATE[STUB]
bool FragShader::Init(
		ID3D11Device*,
		Scaleform::Render::D3D1x::FragShaderDesc const*
	)
{
	// FUNCTION BODY[0x5b6260]
	return false;
}

// STATE[STUB]
void VertexShader::Shutdown( )
{
	// FUNCTION BODY[0x5b62f0]
}

// STATE[STUB]
bool VertexShader::Init(
		ID3D11Device*,
		Scaleform::Render::D3D1x::VertexShaderDesc const*
	)
{
	// FUNCTION BODY[0x5b6310]
	return false;
}

// STATE[STUB]
bool ShaderManager::Initialize( Scaleform::Render::D3D1x::HAL* )
{
	// FUNCTION BODY[0x5b63a0]
	return false;
}

// STATE[STUB]
void ShaderManager::Reset( )
{
	// FUNCTION BODY[0x5b64b0]
}

// STATE[STUB]
void ShaderManager::MapVertexFormat(
		Scaleform::Render::PrimitiveFillType,
		Scaleform::Render::VertexFormat const*,
		Scaleform::Render::VertexFormat const**,
		Scaleform::Render::VertexFormat const**,
		Scaleform::Render::VertexFormat const**
	)
{
	// FUNCTION BODY[0x5b6560]
}

// STATE[STUB]
ShaderManager::ShaderManager( Scaleform::Render::ProfileViews* )
{
	// FUNCTION BODY[0x5b6650]
}

// STATE[STUB]
void ShaderInterface::Finish( unsigned int )
{
	// FUNCTION BODY[0x5b66c0]
}

// STATE[STUB]
SysVertexFormat::SysVertexFormat(
		ID3D11Device*,
		Scaleform::Render::VertexFormat const*,
		Scaleform::Render::D3D1x::VertexShaderDesc const*
	)
{
	// FUNCTION BODY[0x5b6870]
}

// STATE[STUB]
bool ShaderInterface::SetStaticShader(
		Scaleform::Render::D3D1x::ShaderDesc::ShaderType,
		Scaleform::Render::VertexFormat const*
	)
{
	// FUNCTION BODY[0x5b69c0]
	return false;
}

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform
