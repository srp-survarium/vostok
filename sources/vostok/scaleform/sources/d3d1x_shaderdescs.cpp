////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "d3d1x_shaderdescs.h"

namespace Scaleform {
namespace Render {
namespace D3D1x {

// STATE[STUB]
Scaleform::Render::D3D1x::FragShaderDesc::ShaderIndex FragShaderDesc::GetShaderIndex(
		Scaleform::Render::D3D1x::ShaderDesc::ShaderType,
		Scaleform::Render::D3D1x::ShaderDesc::ShaderVersion
	)
{
	// FUNCTION BODY[0x5ac340]
	return ( Scaleform::Render::D3D1x::FragShaderDesc::ShaderIndex )0;
}

// STATE[STUB]
Scaleform::Render::D3D1x::VertexShaderDesc::ShaderIndex VertexShaderDesc::GetShaderIndex(
		Scaleform::Render::D3D1x::ShaderDesc::ShaderType,
		Scaleform::Render::D3D1x::ShaderDesc::ShaderVersion
	)
{
	// FUNCTION BODY[0x5b00c0]
	return ( Scaleform::Render::D3D1x::VertexShaderDesc::ShaderIndex )0;
}

// STATE[STUB]
Scaleform::Render::D3D1x::VertexShaderDesc const* VertexShaderDesc::GetDesc(
		Scaleform::Render::D3D1x::ShaderDesc::ShaderType,
		Scaleform::Render::D3D1x::ShaderDesc::ShaderVersion
	)
{
	// FUNCTION BODY[0x5b3490]
	return 0;
}

// STATE[STUB]
bool ShaderDesc::IsShaderVersionSupported( Scaleform::Render::D3D1x::ShaderDesc::ShaderVersion )
{
	// FUNCTION BODY[0x5b34a0]
	return false;
}

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform
