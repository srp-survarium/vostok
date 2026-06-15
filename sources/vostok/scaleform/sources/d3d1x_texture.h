////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef D3D1X_TEXTURE_H_INCLUDED
#define D3D1X_TEXTURE_H_INCLUDED

// survarium's fork of the Scaleform 4.x D3D1x_Texture.h - the shipped binary
// tracks a NEWER SDK than the vendored 4.0.15 (MappedTextureBase,
// TextureManagerLocks, ...); skeleton rebuilt from the canonical PDB type
// dumps.

#include "Render/Render_Image.h"

#include <d3d11.h>

namespace Scaleform {
namespace Render {
namespace D3D1x {

class MappedTexture;
class TextureManager;

struct TextureFormat : public Render::TextureFormat {
	struct Mapping {
		/* 0x0000 */	ImageFormat		Format;
		/* 0x0004 */	DXGI_FORMAT		D3DFormat;
		/* 0x0008 */	u8				BytesPerPixel;
		/* 0x000c */	void ( *CopyFunc )( pbyte, pcbyte, u32, Palette*, void* );
		/* 0x0010 */	void ( *UncopyFunc )( pbyte, pcbyte, u32, Palette*, void* );
	}; // struct Mapping

	inline					TextureFormat		( Mapping const* arg_0, unsigned long arg_1 ) { /* no source */ }

	// STATE[STUB]
	virtual	ImageFormat		GetImageFormat		( ) const override
	{
		// FUNCTION BODY[0x0b4fd0]
		return pMapping->Format;
	}

	// STATE[STUB]
	virtual	void ( *GetScanlineCopyFn( ) const override )( pbyte, pcbyte, u32, Palette*, void* )
	{
		// FUNCTION BODY[0x0b4fe0]
		return pMapping->CopyFunc;
	}

	// STATE[STUB]
	virtual	void ( *GetScanlineUncopyFn( ) const override )( pbyte, pcbyte, u32, Palette*, void* )
	{
		// FUNCTION BODY[0x0b4ff0]
		return pMapping->UncopyFunc;
	}

	inline	DXGI_FORMAT		GetD3DFormat		( ) const { /* no source */ return pMapping->D3DFormat; }

	virtual					~TextureFormat		( ) { /* no source */ }

public:
	/* 0x0000 */	/* Scaleform::Render::TextureFormat */
	/* 0x0004 */	Mapping const*		pMapping;
	/* 0x0008 */	unsigned long		D3DUsage;
}; // struct TextureFormat

class Texture : public Render::Texture {
public:
	struct HWTextureDesc {
		inline		HWTextureDesc	( ) { /* no source */ }

	public:
		/* 0x0000 */	Size< unsigned long >		Size;
		/* 0x0008 */	ID3D11Texture2D*			pTexture;
		/* 0x000c */	ID3D11ShaderResourceView*	pView;
		/* 0x0010 */	ID3D11Texture2D*			pStagingTexture;
	}; // struct HWTextureDesc

	// everything unmarked is out-of-line in d3d1x_texture.cpp per the rich index
					Texture							(
						TextureManagerLocks*	arg_0,
						TextureFormat const*	arg_1,
						u32						arg_2,
						Size< unsigned long > const&	arg_3,
						u32						arg_4,
						ImageBase*				arg_5
					);
					Texture							(
						TextureManagerLocks*	arg_0,
						ID3D11Texture2D*		arg_1,
						Size< unsigned long >	arg_2,
						ImageBase*				arg_3
					);
	virtual			~Texture						( );

	// STATE[STUB]
	virtual	Size< unsigned long >	GetTextureSize	( u32 arg_0 ) const override
	{
		// FUNCTION BODY[0x0b5030]
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
		return Size< unsigned long >( );
	}

	inline	TextureManager*			GetManager		( ) const { /* no source */ return 0; }

	// STATE[STUB]
	virtual	bool	IsValid							( ) const override
	{
		// FUNCTION BODY[0x0b5050]
		return pTextures != 0;
	}

	virtual	bool	Initialize						( ) override;
			bool	Initialize						( ID3D11Texture2D* arg_0 );

	virtual	void	ReleaseHWTextures				( bool arg_0 ) override;

	virtual	void	ApplyTexture					( u32 arg_0, ImageFillMode const& arg_1 ) override;

	// STATE[STUB]
	virtual	Image*	GetImage						( ) const override
	{
		// FUNCTION BODY[0x0abed0]
		return 0;
	}

	// STATE[STUB]
	virtual	ImageFormat	GetFormat					( ) const override
	{
		// FUNCTION BODY[0x0b5060]
		return GetTextureFormat( )->GetImageFormat( );
	}

	inline	TextureFormat const*	GetTextureFormat		( ) const { /* no source */ return 0; }

	inline	TextureFormat::Mapping const*	GetTextureFormatMapping	( ) const { /* no source */ return 0; }

	virtual	void	GetUVGenMatrix					( Matrix2x4< float >* arg_0 ) const override;

	virtual	bool	Update							( UpdateDesc const* arg_0, u32 arg_1, u32 arg_2 ) override;

	virtual	void	computeUpdateConvertRescaleFlags(
						bool				arg_0,
						bool				arg_1,
						ImageFormat			arg_2,
						ResizeImageType&	arg_3,
						ImageFormat&		arg_4,
						bool&				arg_5
					) override;

private:
	/* 0x0000 */	/* Scaleform::Render::Texture */
	/* 0x0034 */	HWTextureDesc*		pTextures;
	/* 0x0038 */	HWTextureDesc		Texture0;
}; // class Texture

class MappedTexture : public MappedTextureBase {
public:
	inline			MappedTexture	( ) { /* no source */ }

	// Map/Unmap are out-of-line in d3d1x_texture.cpp per the rich index
	virtual	bool	Map				( Render::Texture* arg_0, u32 arg_1, u32 arg_2 ) override;

	virtual	void	Unmap			( bool arg_0 ) override;

	virtual			~MappedTexture	( ) { /* no source */ }
}; // class MappedTexture

class DepthStencilSurface : public Render::DepthStencilSurface {
public:
	// ctor/dtor/Initialize are out-of-line in d3d1x_texture.cpp per the rich index
					DepthStencilSurface	(
						TextureManagerLocks*	arg_0,
						Size< unsigned long > const&	arg_1
					);
	virtual			~DepthStencilSurface( );

	virtual	bool	Initialize			( ) override;

private:
	/* 0x0000 */	/* Scaleform::Render::DepthStencilSurface */
	/* 0x0020 */	ID3D11Texture2D*		pDepthStencilSurface;
	/* 0x0024 */	ID3D11DepthStencilView*	pDepthStencilSurfaceView;
}; // class DepthStencilSurface

// addressed free helpers, out-of-line in d3d1x_texture.cpp per the rich index
bool	IsD3DFormatMipGenCompatible		( DXGI_FORMAT arg_0 );
bool	IsD3DFormatRescaleCompatible	( DXGI_FORMAT arg_0, ImageFormat* arg_1, ResizeImageType* arg_2 );

class TextureManager : public Render::TextureManager {
public:
	// everything unmarked is out-of-line in d3d1x_texture.cpp per the rich index
			void		initTextureFormats			( );

	// STATE[STUB]
	virtual	MappedTextureBase&	getDefaultMappedTexture	( ) override
	{
		// FUNCTION BODY[0x0b50d0]
		return MappedTexture0;
	}

	// STATE[STUB]
	virtual	MappedTextureBase*	createMappedTexture		( ) override
	{
		// FUNCTION BODY[0x0b50e0]
		return 0;
	}

	virtual	void		processTextureKillList		( ) override;

	virtual	void		processInitTextures			( ) override;

						TextureManager				(
							ID3D11Device*			arg_0,
							ID3D11DeviceContext*	arg_1,
							void*					arg_2,
							ThreadCommandQueue*		arg_3,
							TextureCache*			arg_4
						);
	virtual				~TextureManager				( );

			void		Reset						( );

			void		SetSamplerState				(
							u32							arg_0,
							u32							arg_1,
							ID3D11ShaderResourceView**	arg_2,
							ID3D11SamplerState*			arg_3
						);

	virtual	void		BeginScene					( ) override;

	virtual	Render::Texture*	CreateTexture		(
							ImageFormat				arg_0,
							u32						arg_1,
							Size< unsigned long > const&	arg_2,
							u32						arg_3,
							ImageBase*				arg_4,
							MemoryManager*			arg_5
						) override;
	virtual	Render::Texture*	CreateTexture		(
							ID3D11Texture2D*		arg_0,
							Size< unsigned long >	arg_1,
							ImageBase*				arg_2
						);

	virtual	u32			GetTextureUseCaps			( ImageFormat arg_0 ) override;

	inline	bool		IsMultiThreaded				( ) const { /* no source */ return false; }

	virtual	Render::DepthStencilSurface*	CreateDepthStencilSurface	(
							Size< unsigned long > const&	arg_0,
							MemoryManager*			arg_1
						) override;
	virtual	Render::DepthStencilSurface*	CreateDepthStencilSurface	( ID3D11Texture2D* arg_0 );

	// STATE[STUB]
	virtual	bool		IsDrawableImageFormat		( ImageFormat arg_0 ) const override
	{
		// FUNCTION BODY[0x0b5120]
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
		return false;
	}

	inline	ID3D11Device*			GetDevice		( ) const { /* no source */ return pDevice; }

	inline	ID3D11DeviceContext*	GetDeviceContext( ) const { /* no source */ return pDeviceContext; }

private:
	/* 0x0000 */	/* Scaleform::Render::TextureManager */
	/* 0x0058 */	ID3D11Device*			pDevice;
	/* 0x005c */	ID3D11DeviceContext*	pDeviceContext;
	/* 0x0060 */	MappedTexture			MappedTexture0;
	/* 0x00e8 */	ArrayLH< ID3D11Resource*, 75, ArrayConstPolicy< 8, 8, 0 > >	D3DTextureKillList;
	/* 0x00f4 */	ArrayLH< ID3D11View*, 75, ArrayConstPolicy< 8, 8, 0 > >		D3DTexViewKillList;
	/* 0x0100 */	ID3D11SamplerState*		SamplerStates[4];
	/* 0x0110 */	ID3D11SamplerState*		CurrentSamplers[4];
	/* 0x0120 */	ID3D11View*				CurrentTextures[4];
}; // class TextureManager

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform

STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::TextureFormat, 0xC);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::TextureFormat::Mapping, 0x14);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::Texture, 0x4C);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::Texture::HWTextureDesc, 0x14);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::MappedTexture, 0x88);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::DepthStencilSurface, 0x28);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::TextureManager, 0x130);

#endif // #ifndef D3D1X_TEXTURE_H_INCLUDED
