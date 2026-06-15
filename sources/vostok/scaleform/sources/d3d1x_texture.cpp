////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "d3d1x_texture.h"

namespace Scaleform {
namespace Render {
namespace D3D1x {

// STATE[STUB]
unsigned int TextureManager::GetTextureUseCaps( Scaleform::Render::ImageFormat )
{
	// FUNCTION BODY[0x5b34c0]
	return 0;
}

// STATE[STUB]
void TextureManager::processInitTextures( )
{
	// FUNCTION BODY[0x5b3510]
}

// STATE[STUB]
void TextureManager::BeginScene( )
{
	// FUNCTION BODY[0x5b35e0]
}

// STATE[STUB]
void TextureManager::SetSamplerState(
		unsigned int,
		unsigned int,
		ID3D11ShaderResourceView**,
		ID3D11SamplerState*
	)
{
	// FUNCTION BODY[0x5b3660]
}

// STATE[STUB]
bool DepthStencilSurface::Initialize( )
{
	// FUNCTION BODY[0x5b3740]
	return false;
}

// STATE[STUB]
bool IsD3DFormatMipGenCompatible( DXGI_FORMAT )
{
	// FUNCTION BODY[0x5b3800]
	return false;
}

// STATE[STUB]
bool IsD3DFormatRescaleCompatible(
		DXGI_FORMAT,
		Scaleform::Render::ImageFormat*,
		Scaleform::Render::ResizeImageType*
	)
{
	// FUNCTION BODY[0x5b3870]
	return false;
}

// STATE[STUB]
void TextureManager::Reset( )
{
	// FUNCTION BODY[0x5b38b0]
}

// STATE[STUB]
void MappedTexture::Unmap( bool )
{
	// FUNCTION BODY[0x5b3960]
}

// STATE[STUB]
DepthStencilSurface::~DepthStencilSurface( )
{
	// FUNCTION BODY[0x5b3a90]
}

// STATE[STUB]
bool Texture::Update(
		Scaleform::Render::Texture::UpdateDesc const*,
		unsigned int,
		unsigned int
	)
{
	// FUNCTION BODY[0x5b3ad0]
	return false;
}

// STATE[STUB]
void Texture::ApplyTexture( unsigned int, Scaleform::Render::ImageFillMode const& )
{
	// FUNCTION BODY[0x5b3c10]
}

// STATE[STUB]
void Texture::computeUpdateConvertRescaleFlags(
		bool,
		bool,
		Scaleform::Render::ImageFormat,
		Scaleform::Render::ResizeImageType&,
		Scaleform::Render::ImageFormat&,
		bool&
	)
{
	// FUNCTION BODY[0x5b3c70]
}

// STATE[STUB]
void TextureManager::processTextureKillList( )
{
	// FUNCTION BODY[0x5b3cf0]
}

// STATE[STUB]
void TextureManager::initTextureFormats( )
{
	// FUNCTION BODY[0x5b3e30]
}

// STATE[STUB]
bool MappedTexture::Map(
		Scaleform::Render::Texture*,
		unsigned int,
		unsigned int
	)
{
	// FUNCTION BODY[0x5b3ef0]
	return false;
}

// STATE[STUB]
void Texture::ReleaseHWTextures( bool )
{
	// FUNCTION BODY[0x5b42f0]
}

// STATE[STUB]
bool Texture::Initialize( ID3D11Texture2D* )
{
	// FUNCTION BODY[0x5b44d0]
	return false;
}

// STATE[STUB]
bool Texture::Initialize( )
{
	// FUNCTION BODY[0x5b4600]
	return false;
}

// STATE[STUB]
Texture::~Texture( )
{
	// FUNCTION BODY[0x5b4830]
}

// STATE[STUB]
DepthStencilSurface::DepthStencilSurface(
		Scaleform::Render::TextureManagerLocks*,
		Scaleform::Render::Size<unsigned long> const&
	)
{
	// FUNCTION BODY[0x5b48c0]
}

// STATE[STUB]
void Texture::GetUVGenMatrix( Scaleform::Render::Matrix2x4<float>* ) const
{
	// FUNCTION BODY[0x5b4920]
}

// STATE[STUB]
Scaleform::Render::DepthStencilSurface* TextureManager::CreateDepthStencilSurface( ID3D11Texture2D* )
{
	// FUNCTION BODY[0x5b4990]
	return 0;
}

// STATE[STUB]
Scaleform::Render::DepthStencilSurface* TextureManager::CreateDepthStencilSurface(
		Scaleform::Render::Size<unsigned long> const&,
		Scaleform::Render::MemoryManager*
	)
{
	// FUNCTION BODY[0x5b4a30]
	return 0;
}

// STATE[STUB]
Texture::Texture(
		Scaleform::Render::TextureManagerLocks*,
		ID3D11Texture2D*,
		Scaleform::Render::Size<unsigned long>,
		Scaleform::Render::ImageBase*
	)
{
	// FUNCTION BODY[0x5b4aa0]
}

// STATE[STUB]
Texture::Texture(
		Scaleform::Render::TextureManagerLocks*,
		Scaleform::Render::D3D1x::TextureFormat const*,
		unsigned int,
		Scaleform::Render::Size<unsigned long> const&,
		unsigned int,
		Scaleform::Render::ImageBase*
	)
{
	// FUNCTION BODY[0x5b4b00]
}

// STATE[STUB]
Scaleform::Render::Texture* TextureManager::CreateTexture(
		ID3D11Texture2D*,
		Scaleform::Render::Size<unsigned long>,
		Scaleform::Render::ImageBase*
	)
{
	// FUNCTION BODY[0x5b4b80]
	return 0;
}

// STATE[STUB]
Scaleform::Render::Texture* TextureManager::CreateTexture(
		Scaleform::Render::ImageFormat,
		unsigned int,
		Scaleform::Render::Size<unsigned long> const&,
		unsigned int,
		Scaleform::Render::ImageBase*,
		Scaleform::Render::MemoryManager*
	)
{
	// FUNCTION BODY[0x5b4c20]
	return 0;
}

// STATE[STUB]
TextureManager::~TextureManager( )
{
	// FUNCTION BODY[0x5b4cd0]
}

// STATE[STUB]
TextureManager::TextureManager(
		ID3D11Device*,
		ID3D11DeviceContext*,
		void*,
		Scaleform::Render::ThreadCommandQueue*,
		Scaleform::Render::TextureCache*
	)
{
	// FUNCTION BODY[0x5b4d70]
}

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform
