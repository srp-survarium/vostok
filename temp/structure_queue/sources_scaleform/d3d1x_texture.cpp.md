# vostok/scaleform/sources/d3d1x_texture.cpp - compiland MISSING from binaries/structure/target (parser gap)
#
# 30 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x5b34c0 size 0x4f    unsigned int Scaleform::Render::D3D1x::TextureManager::GetTextureUseCaps(Scaleform::Render::ImageFormat)
    ?GetTextureUseCaps@TextureManager@D3D1x@Render@Scaleform@@UAEIW4ImageFormat@34@@Z
0x5b3510 size 0xcb    void Scaleform::Render::D3D1x::TextureManager::processInitTextures()
    ?processInitTextures@TextureManager@D3D1x@Render@Scaleform@@EAEXXZ
0x5b35e0 size 0x7b    void Scaleform::Render::D3D1x::TextureManager::BeginScene()
    ?BeginScene@TextureManager@D3D1x@Render@Scaleform@@UAEXXZ
0x5b3660 size 0xd5    void Scaleform::Render::D3D1x::TextureManager::SetSamplerState(unsigned int, unsigned int, ID3D11ShaderResourceView**, ID3D11SamplerState*)
    ?SetSamplerState@TextureManager@D3D1x@Render@Scaleform@@QAEXIIPAPAUID3D11ShaderResourceView@@PAUID3D11SamplerState@@@Z
0x5b3740 size 0xb8    bool Scaleform::Render::D3D1x::DepthStencilSurface::Initialize()
    ?Initialize@DepthStencilSurface@D3D1x@Render@Scaleform@@UAE_NXZ
0x5b3800 size 0x64    bool Scaleform::Render::D3D1x::IsD3DFormatMipGenCompatible(DXGI_FORMAT)
    ?IsD3DFormatMipGenCompatible@D3D1x@Render@Scaleform@@YA_NW4DXGI_FORMAT@@@Z
0x5b3870 size 0x34    bool Scaleform::Render::D3D1x::IsD3DFormatRescaleCompatible(DXGI_FORMAT, Scaleform::Render::ImageFormat*, Scaleform::Render::ResizeImageType*)
    ?IsD3DFormatRescaleCompatible@D3D1x@Render@Scaleform@@YA_NW4DXGI_FORMAT@@PAW4ImageFormat@23@PAW4ResizeImageType@23@@Z
0x5b38b0 size 0xa2    void Scaleform::Render::D3D1x::TextureManager::Reset()
    ?Reset@TextureManager@D3D1x@Render@Scaleform@@QAEXXZ
0x5b3960 size 0x130   void Scaleform::Render::D3D1x::MappedTexture::Unmap(bool)
    ?Unmap@MappedTexture@D3D1x@Render@Scaleform@@UAEX_N@Z
0x5b3a90 size 0x37    void Scaleform::Render::D3D1x::DepthStencilSurface::~DepthStencilSurface()
    ??1DepthStencilSurface@D3D1x@Render@Scaleform@@UAE@XZ
0x5b3ad0 size 0x135   bool Scaleform::Render::D3D1x::Texture::Update(Scaleform::Render::Texture::UpdateDesc const*, unsigned int, unsigned int)
    ?Update@Texture@D3D1x@Render@Scaleform@@UAE_NPBUUpdateDesc@134@II@Z
0x5b3c10 size 0x5d    void Scaleform::Render::D3D1x::Texture::ApplyTexture(unsigned int, Scaleform::Render::ImageFillMode const&)
    ?ApplyTexture@Texture@D3D1x@Render@Scaleform@@UAEXIABUImageFillMode@34@@Z
0x5b3c70 size 0x71    void Scaleform::Render::D3D1x::Texture::computeUpdateConvertRescaleFlags(bool, bool, Scaleform::Render::ImageFormat, Scaleform::Render::ResizeImageType&, Scaleform::Render::ImageFormat&, bool&)
    ?computeUpdateConvertRescaleFlags@Texture@D3D1x@Render@Scaleform@@MAEX_N0W4ImageFormat@34@AAW4ResizeImageType@34@AAW4534@AA_N@Z
0x5b3cf0 size 0x139   void Scaleform::Render::D3D1x::TextureManager::processTextureKillList()
    ?processTextureKillList@TextureManager@D3D1x@Render@Scaleform@@EAEXXZ
0x5b3e30 size 0xbe    void Scaleform::Render::D3D1x::TextureManager::initTextureFormats()
    ?initTextureFormats@TextureManager@D3D1x@Render@Scaleform@@AAEXXZ
0x5b3ef0 size 0x3fc   bool Scaleform::Render::D3D1x::MappedTexture::Map(Scaleform::Render::Texture*, unsigned int, unsigned int)
    ?Map@MappedTexture@D3D1x@Render@Scaleform@@UAE_NPAVTexture@34@II@Z
0x5b42f0 size 0x1d3   void Scaleform::Render::D3D1x::Texture::ReleaseHWTextures(bool)
    ?ReleaseHWTextures@Texture@D3D1x@Render@Scaleform@@UAEX_N@Z
0x5b44d0 size 0x121   bool Scaleform::Render::D3D1x::Texture::Initialize(ID3D11Texture2D*)
    ?Initialize@Texture@D3D1x@Render@Scaleform@@QAE_NPAUID3D11Texture2D@@@Z
0x5b4600 size 0x22c   bool Scaleform::Render::D3D1x::Texture::Initialize()
    ?Initialize@Texture@D3D1x@Render@Scaleform@@UAE_NXZ
0x5b4830 size 0x87    void Scaleform::Render::D3D1x::Texture::~Texture()
    ??1Texture@D3D1x@Render@Scaleform@@UAE@XZ
0x5b48c0 size 0x52    Scaleform::Render::D3D1x::DepthStencilSurface::DepthStencilSurface(Scaleform::Render::TextureManagerLocks*, Scaleform::Render::Size<unsigned long> const&)
    ??0DepthStencilSurface@D3D1x@Render@Scaleform@@QAE@PAVTextureManagerLocks@23@ABV?$Size@K@23@@Z
0x5b4920 size 0x6c    void Scaleform::Render::D3D1x::Texture::GetUVGenMatrix(Scaleform::Render::Matrix2x4<float>*) const
    ?GetUVGenMatrix@Texture@D3D1x@Render@Scaleform@@UBEXPAV?$Matrix2x4@M@34@@Z
0x5b4990 size 0x98    Scaleform::Render::DepthStencilSurface* Scaleform::Render::D3D1x::TextureManager::CreateDepthStencilSurface(ID3D11Texture2D*)
    ?CreateDepthStencilSurface@TextureManager@D3D1x@Render@Scaleform@@UAEPAVDepthStencilSurface@34@PAUID3D11Texture2D@@@Z
0x5b4a30 size 0x61    Scaleform::Render::DepthStencilSurface* Scaleform::Render::D3D1x::TextureManager::CreateDepthStencilSurface(Scaleform::Render::Size<unsigned long> const&, Scaleform::Render::MemoryManager*)
    ?CreateDepthStencilSurface@TextureManager@D3D1x@Render@Scaleform@@UAEPAVDepthStencilSurface@34@ABV?$Size@K@34@PAVMemoryManager@34@@Z
0x5b4aa0 size 0x5f    Scaleform::Render::D3D1x::Texture::Texture(Scaleform::Render::TextureManagerLocks*, ID3D11Texture2D*, Scaleform::Render::Size<unsigned long>, Scaleform::Render::ImageBase*)
    ??0Texture@D3D1x@Render@Scaleform@@QAE@PAVTextureManagerLocks@23@PAUID3D11Texture2D@@V?$Size@K@23@PAVImageBase@23@@Z
0x5b4b00 size 0x7f    Scaleform::Render::D3D1x::Texture::Texture(Scaleform::Render::TextureManagerLocks*, Scaleform::Render::D3D1x::TextureFormat const*, unsigned int, Scaleform::Render::Size<unsigned long> const&, unsigned int, Scaleform::Render::ImageBase*)
    ??0Texture@D3D1x@Render@Scaleform@@QAE@PAVTextureManagerLocks@23@PBUTextureFormat@123@IABV?$Size@K@23@IPAVImageBase@23@@Z
0x5b4b80 size 0x98    Scaleform::Render::Texture* Scaleform::Render::D3D1x::TextureManager::CreateTexture(ID3D11Texture2D*, Scaleform::Render::Size<unsigned long>, Scaleform::Render::ImageBase*)
    ?CreateTexture@TextureManager@D3D1x@Render@Scaleform@@UAEPAVTexture@34@PAUID3D11Texture2D@@V?$Size@K@34@PAVImageBase@34@@Z
0x5b4c20 size 0xab    Scaleform::Render::Texture* Scaleform::Render::D3D1x::TextureManager::CreateTexture(Scaleform::Render::ImageFormat, unsigned int, Scaleform::Render::Size<unsigned long> const&, unsigned int, Scaleform::Render::ImageBase*, Scaleform::Render::MemoryManager*)
    ?CreateTexture@TextureManager@D3D1x@Render@Scaleform@@UAEPAVTexture@34@W4ImageFormat@34@IABV?$Size@K@34@IPAVImageBase@34@PAVMemoryManager@34@@Z
0x5b4cd0 size 0x99    void Scaleform::Render::D3D1x::TextureManager::~TextureManager()
    ??1TextureManager@D3D1x@Render@Scaleform@@UAE@XZ
0x5b4d70 size 0x13f   Scaleform::Render::D3D1x::TextureManager::TextureManager(ID3D11Device*, ID3D11DeviceContext*, void*, Scaleform::Render::ThreadCommandQueue*, Scaleform::Render::TextureCache*)
    ??0TextureManager@D3D1x@Render@Scaleform@@QAE@PAUID3D11Device@@PAUID3D11DeviceContext@@PAXPAVThreadCommandQueue@23@PAVTextureCache@23@@Z
