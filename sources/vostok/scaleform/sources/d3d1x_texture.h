////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef D3D1X_TEXTURE_H_INCLUDED
#define D3D1X_TEXTURE_H_INCLUDED

// survarium's fork of the Scaleform 4.x D3D1x_Texture.h - the shipped binary
// tracks a NEWER SDK than the vendored 4.0.15 (MappedTextureBase,
// TextureManagerLocks, ...); skeleton rebuilt from the canonical PDB type
// dumps.

#include "Render/D3D1x/D3D1x_Config.h"
#include "Render/Render_Image.h"
#include "Kernel/SF_HeapNew.h"

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
		/* 0x000c */	Image::CopyScanlineFunc	CopyFunc;
		/* 0x0010 */	Image::CopyScanlineFunc	UncopyFunc;
	}; // struct Mapping

	inline					TextureFormat		( Mapping const* arg_0, unsigned long arg_1 ) : pMapping(arg_0), D3DUsage(arg_1) { }

	virtual	ImageFormat		GetImageFormat		( ) const override { return pMapping->Format; }
	virtual	Image::CopyScanlineFunc GetScanlineCopyFn( ) const override { return pMapping->CopyFunc; }
	virtual	Image::CopyScanlineFunc GetScanlineUncopyFn( ) const override { return pMapping->UncopyFunc; }

	inline	DXGI_FORMAT		GetD3DFormat		( ) const { return pMapping->D3DFormat; }

public:
	/* 0x0000 */	/* Scaleform::Render::TextureFormat */
	/* 0x0004 */	Mapping const*		pMapping;
	/* 0x0008 */	unsigned long		D3DUsage;
}; // struct TextureFormat

class Texture : public Render::Texture {
public:
	static const u8 MaxTextureCount = 4;

	struct HWTextureDesc {
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

	virtual	Size< unsigned long >	GetTextureSize	( u32 arg_0 ) const override { return pTextures[arg_0].Size; }

	inline	TextureManager*			GetManager		( ) const { return (TextureManager*)pManagerLocks->pManager; }

	virtual	bool	IsValid							( ) const override { return pTextures != 0; }

	virtual	bool	Initialize						( ) override;
			bool	Initialize						( ID3D11Texture2D* arg_0 );

	virtual	void	ReleaseHWTextures				( bool arg_0 = true ) override;

	virtual	void	ApplyTexture					( u32 arg_0, ImageFillMode const& arg_1 ) override;

	virtual	Image*	GetImage						( ) const override { SF_ASSERT(!pImage || (pImage->GetImageType() != Image::Type_ImageBase)); return (Image*)pImage; }

	virtual	ImageFormat	GetFormat					( ) const override { return GetImageFormat( ); }

	inline	TextureFormat const*	GetTextureFormat		( ) const { return reinterpret_cast<TextureFormat const*>(pFormat); }

	inline	TextureFormat::Mapping const*	GetTextureFormatMapping	( ) const { return pFormat ? reinterpret_cast<TextureFormat const*>(pFormat)->pMapping : 0; }

	virtual	void	GetUVGenMatrix					( Matrix2x4< float >* arg_0 ) const override;

	virtual	bool	Update							( UpdateDesc const* arg_0, u32 arg_1, u32 arg_2 ) override;

protected:
	virtual	void	computeUpdateConvertRescaleFlags(
						bool				arg_0,
						bool				arg_1,
						ImageFormat			arg_2,
						ResizeImageType&	arg_3,
						ImageFormat&		arg_4,
						bool&				arg_5
					) override;

public:
	/* 0x0000 */	/* Scaleform::Render::Texture */
	/* 0x0034 */	HWTextureDesc*		pTextures;
	/* 0x0038 */	HWTextureDesc		Texture0;
}; // class Texture

class MappedTexture : public MappedTextureBase {
public:
	inline			MappedTexture	( ) : MappedTextureBase( ) { }

	// Map/Unmap are out-of-line in d3d1x_texture.cpp per the rich index
	virtual	bool	Map				( Render::Texture* arg_0, u32 arg_1, u32 arg_2 ) override;

	virtual	void	Unmap			( bool arg_0 ) override;

}; // class MappedTexture

class DepthStencilSurface : public Render::DepthStencilSurface {
public:
	// ctor/dtor/Initialize are out-of-line in d3d1x_texture.cpp per the rich index
					DepthStencilSurface	(
						TextureManagerLocks*	arg_0,
						Render::Size< unsigned long > const&	arg_1
					);
	virtual			~DepthStencilSurface( );

	virtual	bool	Initialize			( ) override;

public:
	/* 0x0000 */	/* Scaleform::Render::DepthStencilSurface */
	/* 0x0020 */	ID3D11Texture2D*		pDepthStencilSurface;
	/* 0x0024 */	ID3D11DepthStencilView*	pDepthStencilSurfaceView;
}; // class DepthStencilSurface

// addressed free helpers, out-of-line in d3d1x_texture.cpp per the rich index
bool	IsD3DFormatMipGenCompatible		( DXGI_FORMAT arg_0 );
bool	IsD3DFormatRescaleCompatible	( DXGI_FORMAT arg_0, ImageFormat* arg_1, ResizeImageType* arg_2 );

class TextureManager : public Render::TextureManager {
    friend class Texture;
    friend class DepthStencilSurface;

private:
	// everything unmarked is out-of-line in d3d1x_texture.cpp per the rich index
			void		initTextureFormats			( );

	virtual	MappedTextureBase&	getDefaultMappedTexture	( ) override { return MappedTexture0; }
	virtual	MappedTextureBase*	createMappedTexture		( ) override { return SF_HEAP_AUTO_NEW(this) MappedTexture; }

	virtual	void		processTextureKillList		( ) override;

	virtual	void		processInitTextures			( ) override;

public:
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

	inline	bool		IsMultiThreaded				( ) const { return RenderThreadId != 0; }

	virtual	Render::DepthStencilSurface*	CreateDepthStencilSurface	(
							Size< unsigned long > const&	arg_0,
							MemoryManager*			arg_1
						) override;
	virtual	Render::DepthStencilSurface*	CreateDepthStencilSurface	( ID3D11Texture2D* arg_0 );

	virtual	bool		IsDrawableImageFormat		( ImageFormat arg_0 ) const override { return (arg_0 == Image_B8G8R8A8) || (arg_0 == Image_R8G8B8A8); }

	inline	ID3D11Device*			GetDevice		( ) const { return pDevice; }

	inline	ID3D11DeviceContext*	GetDeviceContext( ) const { return pDeviceContext; }

private:
	/* 0x0000 */	/* Scaleform::Render::TextureManager */
	/* 0x0058 */	ID3D11Device*			pDevice;
	/* 0x005c */	ID3D11DeviceContext*	pDeviceContext;
	/* 0x0060 */	MappedTexture			MappedTexture0;
	/* 0x00e8 */	ArrayLH< ID3D11Resource*, 75, ArrayConstPolicy< 8, 8, 0 > >	D3DTextureKillList;
	/* 0x00f4 */	ArrayLH< ID3D11View*, 75, ArrayConstPolicy< 8, 8, 0 > >		D3DTexViewKillList;
	/* 0x0100 */	ID3D11SamplerState*		SamplerStates[4];
	static const int MaximumStages = 4;
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
