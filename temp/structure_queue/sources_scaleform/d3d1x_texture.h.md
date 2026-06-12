# vostok/scaleform/sources/d3d1x_texture.h - compiland MISSING from binaries/structure/target (parser gap)
#
# 17 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x0abed0 size 0x4     Scaleform::Render::Image* Scaleform::Render::D3D1x::Texture::GetImage() const
    ?GetImage@Texture@D3D1x@Render@Scaleform@@UBEPAVImage@34@XZ
0x0b4fd0 size 0x6     Scaleform::Render::ImageFormat Scaleform::Render::D3D1x::TextureFormat::GetImageFormat() const
    ?GetImageFormat@TextureFormat@D3D1x@Render@Scaleform@@UBE?AW4ImageFormat@34@XZ
0x0b4fe0 size 0x7     void (*)(unsigned char*, unsigned char const*, unsigned int, Scaleform::Render::Palette*, void*) Scaleform::Render::D3D1x::TextureFormat::GetScanlineCopyFn() const
    ?GetScanlineCopyFn@TextureFormat@D3D1x@Render@Scaleform@@UBEP6GXPAEPBEIPAVPalette@34@PAX@ZXZ
0x0b4ff0 size 0x7     void (*)(unsigned char*, unsigned char const*, unsigned int, Scaleform::Render::Palette*, void*) Scaleform::Render::D3D1x::TextureFormat::GetScanlineUncopyFn() const
    ?GetScanlineUncopyFn@TextureFormat@D3D1x@Render@Scaleform@@UBEP6GXPAEPBEIPAVPalette@34@PAX@ZXZ
0x0b5000 size 0x24    void* Scaleform::Render::D3D1x::TextureFormat::`scalar deleting destructor'(unsigned int)
    ??_GTextureFormat@Render@Scaleform@@UAEPAXI@Z
0x0b5000 size 0x24    void* Scaleform::Render::TextureFormat::`scalar deleting destructor'(unsigned int)
    ??_GTextureFormat@Render@Scaleform@@UAEPAXI@Z
0x0b5030 size 0x1f    Scaleform::Render::Size<unsigned long> Scaleform::Render::D3D1x::Texture::GetTextureSize(unsigned int) const
    ?GetTextureSize@Texture@D3D1x@Render@Scaleform@@UBE?AV?$Size@K@34@I@Z
0x0b5050 size 0x9     bool Scaleform::Render::D3D1x::Texture::IsValid() const
    ?IsValid@Texture@D3D1x@Render@Scaleform@@UBE_NXZ
0x0b5060 size 0x7     Scaleform::Render::ImageFormat Scaleform::Render::D3D1x::Texture::GetFormat() const
    ?GetFormat@Texture@D3D1x@Render@Scaleform@@UBE?AW4ImageFormat@34@XZ
0x0b5070 size 0x53    void* Scaleform::Render::D3D1x::MappedTexture::`scalar deleting destructor'(unsigned int)
    ??_GMappedTextureBase@Render@Scaleform@@UAEPAXI@Z
0x0b5070 size 0x53    void* Scaleform::Render::MappedTextureBase::`scalar deleting destructor'(unsigned int)
    ??_GMappedTextureBase@Render@Scaleform@@UAEPAXI@Z
0x0b50d0 size 0x4     Scaleform::Render::MappedTextureBase& Scaleform::Render::D3D1x::TextureManager::getDefaultMappedTexture()
    ?getDefaultMappedTexture@TextureManager@D3D1x@Render@Scaleform@@EAEAAVMappedTextureBase@34@XZ
0x0b50e0 size 0x3f    Scaleform::Render::MappedTextureBase* Scaleform::Render::D3D1x::TextureManager::createMappedTexture()
    ?createMappedTexture@TextureManager@D3D1x@Render@Scaleform@@EAEPAVMappedTextureBase@34@XZ
0x0b5120 size 0x1b    bool Scaleform::Render::D3D1x::TextureManager::IsDrawableImageFormat(Scaleform::Render::ImageFormat) const
    ?IsDrawableImageFormat@TextureManager@D3D1x@Render@Scaleform@@UBE_NW4ImageFormat@34@@Z
0x0b5140 size 0x23    void* Scaleform::Render::D3D1x::Texture::`scalar deleting destructor'(unsigned int)
    ??_GTexture@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b5170 size 0x55    void* Scaleform::Render::D3D1x::DepthStencilSurface::`scalar deleting destructor'(unsigned int)
    ??_GDepthStencilSurface@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b51d0 size 0x25    void* Scaleform::Render::D3D1x::TextureManager::`scalar deleting destructor'(unsigned int)
    ??_GTextureManager@D3D1x@Render@Scaleform@@UAEPAXI@Z
