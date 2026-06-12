# vostok/scaleform/sources/d3d1x_hal.h - compiland MISSING from binaries/structure/target (parser gap)
#
# 14 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x0aed60 size 0x19    void Scaleform::Render::D3D1x::HAL::beginMaskDisplay()
    ?beginMaskDisplay@HAL@D3D1x@Render@Scaleform@@UAEXXZ
0x0aed80 size 0x7     Scaleform::Render::TextureManager* Scaleform::Render::D3D1x::HAL::GetTextureManager() const
    ?GetTextureManager@HAL@D3D1x@Render@Scaleform@@UBEPAVTextureManager@34@XZ
0x0aed90 size 0x7     Scaleform::Render::D3D1x::MeshCache& Scaleform::Render::D3D1x::HAL::GetMeshCache()
    ?GetMeshCache@HAL@D3D1x@Render@Scaleform@@UAEAAVMeshCache@234@XZ
0x0aeda0 size 0x27    void Scaleform::Render::D3D1x::HAL::MapVertexFormat(Scaleform::Render::PrimitiveFillType, Scaleform::Render::VertexFormat const*, Scaleform::Render::VertexFormat const**, Scaleform::Render::VertexFormat const**, Scaleform::Render::VertexFormat const**, unsigned int)
    ?MapVertexFormat@HAL@D3D1x@Render@Scaleform@@UAEXW4PrimitiveFillType@34@PBUVertexFormat@34@PAPBU634@22I@Z
0x0aedd0 size 0xb6    static void Scaleform::Render::D3D1x::RenderTargetData::UpdateData(Scaleform::Render::RenderBuffer*, ID3D11View*, Scaleform::Render::DepthStencilBuffer*, ID3D11View*)
    ?UpdateData@RenderTargetData@D3D1x@Render@Scaleform@@SAXPAVRenderBuffer@34@PAUID3D11View@@PAVDepthStencilBuffer@34@1@Z
0x0aee90 size 0x56    Scaleform::Render::D3D1x::RenderTargetData::RenderTargetData(Scaleform::Render::RenderBuffer*, ID3D11View*, Scaleform::Render::DepthStencilBuffer*, ID3D11View*)
    ??0RenderTargetData@D3D1x@Render@Scaleform@@AAE@PAVRenderBuffer@23@PAUID3D11View@@PAVDepthStencilBuffer@23@1@Z
0x0aeef0 size 0x56    void* Scaleform::Render::D3D1x::RenderTargetData::`scalar deleting destructor'(unsigned int)
    ??_GRenderTargetData@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0aef50 size 0x23    void* Scaleform::Render::D3D1x::HAL::`scalar deleting destructor'(unsigned int)
    ??_GHAL@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0aef80 size 0x10c   void Scaleform::Render::HAL::~HAL()
    ??1HAL@Render@Scaleform@@UAE@XZ
0x0af090 size 0x9d    void Scaleform::Render::D3D1x::ShaderManager::~ShaderManager()
    ??1ShaderManager@D3D1x@Render@Scaleform@@QAE@XZ
0x0af130 size 0x74    Scaleform::Render::HAL::RenderTargetEntry::RenderTargetEntry()
    ??0RenderTargetEntry@HAL@Render@Scaleform@@QAE@XZ
0x0af1b0 size 0xc3    Scaleform::Render::HAL::RenderTargetEntry& Scaleform::Render::HAL::RenderTargetEntry::operator=(Scaleform::Render::HAL::RenderTargetEntry const&)
    ??4RenderTargetEntry@HAL@Render@Scaleform@@QAEAAU0123@ABU0123@@Z
0x0af280 size 0x2fe   Scaleform::Render::MatrixState& Scaleform::Render::MatrixState::operator=(Scaleform::Render::MatrixState const&)
    ??4MatrixState@Render@Scaleform@@QAEAAV012@ABV012@@Z
0x0af580 size 0x1e    void Scaleform::Render::HAL::FilterStackEntry::~FilterStackEntry()
    ??1FilterStackEntry@HAL@Render@Scaleform@@QAE@XZ
