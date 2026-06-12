# vostok/scaleform/sources/d3d1x_hal.cpp - compiland MISSING from binaries/structure/target (parser gap)
#
# 45 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x5b6ac0 size 0x42    void Scaleform::Render::D3D1x::HAL::drawScreenQuad()
    ?drawScreenQuad@HAL@D3D1x@Render@Scaleform@@MAEXXZ
0x5b6b10 size 0x23    void Scaleform::Render::D3D1x::HAL::drawIndexedInstanced(unsigned int, unsigned int, unsigned int, unsigned int)
    ?drawIndexedInstanced@HAL@D3D1x@Render@Scaleform@@IAEXIIII@Z
0x5b6b40 size 0x1c    void Scaleform::Render::D3D1x::HAL::drawIndexedPrimitive(unsigned int, unsigned int, unsigned int, unsigned int)
    ?drawIndexedPrimitive@HAL@D3D1x@Render@Scaleform@@IAEXIIII@Z
0x5b6b60 size 0x18    void Scaleform::Render::D3D1x::HAL::drawPrimitive(unsigned int, unsigned int)
    ?drawPrimitive@HAL@D3D1x@Render@Scaleform@@MAEXII@Z
0x5b6b80 size 0x2e    void Scaleform::Render::D3D1x::HAL::setBatchUnitSquareVertexStream()
    ?setBatchUnitSquareVertexStream@HAL@D3D1x@Render@Scaleform@@MAEXXZ
0x5b6bb0 size 0x18    ID3D11Buffer* Scaleform::Render::D3D1x::HAL::getNextConstantBuffer()
    ?getNextConstantBuffer@HAL@D3D1x@Render@Scaleform@@QAEPAUID3D11Buffer@@XZ
0x5b6bd0 size 0x48    void Scaleform::Render::D3D1x::HAL::destroyConstantBuffers()
    ?destroyConstantBuffers@HAL@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6c20 size 0x99    bool Scaleform::Render::D3D1x::HAL::createConstantBuffers()
    ?createConstantBuffers@HAL@D3D1x@Render@Scaleform@@QAE_NXZ
0x5b6cc0 size 0x39    void Scaleform::Render::D3D1x::HAL::destroyRasterStates()
    ?destroyRasterStates@HAL@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6d00 size 0x8a    bool Scaleform::Render::D3D1x::HAL::createRasterStates()
    ?createRasterStates@HAL@D3D1x@Render@Scaleform@@QAE_NXZ
0x5b6d90 size 0x48    void Scaleform::Render::D3D1x::HAL::destroyDepthStencilStates()
    ?destroyDepthStencilStates@HAL@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6de0 size 0x18c   bool Scaleform::Render::D3D1x::HAL::createDepthStencilStates()
    ?createDepthStencilStates@HAL@D3D1x@Render@Scaleform@@QAE_NXZ
0x5b6f70 size 0x40    void Scaleform::Render::D3D1x::HAL::destroyBlendStates()
    ?destroyBlendStates@HAL@D3D1x@Render@Scaleform@@QAEXXZ
0x5b6fb0 size 0x11e   bool Scaleform::Render::D3D1x::HAL::createBlendStates()
    ?createBlendStates@HAL@D3D1x@Render@Scaleform@@QAE_NXZ
0x5b70d0 size 0x108   bool Scaleform::Render::D3D1x::HAL::checkMaskBufferCaps()
    ?checkMaskBufferCaps@HAL@D3D1x@Render@Scaleform@@QAE_NXZ
0x5b71e0 size 0x3e    bool Scaleform::Render::D3D1x::HAL::shouldRenderFilters(Scaleform::Render::FilterPrimitive const*) const
    ?shouldRenderFilters@HAL@D3D1x@Render@Scaleform@@MBE_NPBVFilterPrimitive@34@@Z
0x5b7220 size 0x135   void Scaleform::Render::D3D1x::HAL::updateViewport()
    ?updateViewport@HAL@D3D1x@Render@Scaleform@@UAEXXZ
0x5b7360 size 0x7f    bool Scaleform::Render::D3D1x::HAL::RestoreAfterReset()
    ?RestoreAfterReset@HAL@D3D1x@Render@Scaleform@@QAE_NXZ
0x5b73e0 size 0x32    Scaleform::Render::RenderEvent& Scaleform::Render::D3D1x::HAL::GetEvent(Scaleform::Render::EventType)
    ?GetEvent@HAL@D3D1x@Render@Scaleform@@MAEAAVRenderEvent@34@W4EventType@34@@Z
0x5b7420 size 0x114   Scaleform::Render::RenderTarget* Scaleform::Render::D3D1x::HAL::CreateTempRenderTarget(Scaleform::Render::Size<unsigned long> const&, bool)
    ?CreateTempRenderTarget@HAL@D3D1x@Render@Scaleform@@UAEPAVRenderTarget@34@ABV?$Size@K@34@_N@Z
0x5b7540 size 0x125   Scaleform::Render::RenderTarget* Scaleform::Render::D3D1x::HAL::CreateRenderTarget(Scaleform::Render::Texture*, bool)
    ?CreateRenderTarget@HAL@D3D1x@Render@Scaleform@@UAEPAVRenderTarget@34@PAVTexture@34@_N@Z
0x5b7670 size 0x158   Scaleform::Render::RenderTarget* Scaleform::Render::D3D1x::HAL::CreateRenderTarget(ID3D11View*, ID3D11View*)
    ?CreateRenderTarget@HAL@D3D1x@Render@Scaleform@@UAEPAVRenderTarget@34@PAUID3D11View@@0@Z
0x5b77d0 size 0xf5    bool Scaleform::Render::D3D1x::HAL::ShutdownHAL()
    ?ShutdownHAL@HAL@D3D1x@Render@Scaleform@@UAE_NXZ
0x5b78d0 size 0x247   bool Scaleform::Render::D3D1x::HAL::createDefaultRenderBuffer()
    ?createDefaultRenderBuffer@HAL@D3D1x@Render@Scaleform@@UAE_NXZ
0x5b7b20 size 0xf5    bool Scaleform::Render::D3D1x::HAL::SetRenderTarget(Scaleform::Render::RenderTarget*, bool)
    ?SetRenderTarget@HAL@D3D1x@Render@Scaleform@@UAE_NPAVRenderTarget@34@_N@Z
0x5b7c20 size 0x5b7   void Scaleform::Render::D3D1x::HAL::drawCachedFilter(Scaleform::Render::FilterPrimitive*)
    ?drawCachedFilter@HAL@D3D1x@Render@Scaleform@@UAEXPAVFilterPrimitive@34@@Z
0x5b81e0 size 0x67f   void Scaleform::Render::D3D1x::HAL::drawUncachedFilter(Scaleform::Render::HAL::FilterStackEntry const&)
    ?drawUncachedFilter@HAL@D3D1x@Render@Scaleform@@UAEXABUFilterStackEntry@134@@Z
0x5b8860 size 0x2c8   void Scaleform::Render::D3D1x::HAL::PushFilters(Scaleform::Render::FilterPrimitive*)
    ?PushFilters@HAL@D3D1x@Render@Scaleform@@UAEXPAVFilterPrimitive@34@@Z
0x5b8b30 size 0x1ff   void Scaleform::Render::D3D1x::HAL::PopRenderTarget(unsigned int)
    ?PopRenderTarget@HAL@D3D1x@Render@Scaleform@@UAEXI@Z
0x5b8d30 size 0x3a6   void Scaleform::Render::D3D1x::HAL::PushRenderTarget(Scaleform::Render::Rect<float> const&, Scaleform::Render::RenderTarget*, unsigned int)
    ?PushRenderTarget@HAL@D3D1x@Render@Scaleform@@UAEXABV?$Rect@M@34@PAVRenderTarget@34@I@Z
0x5b90e0 size 0xb9    void Scaleform::Render::D3D1x::HAL::applyBlendModeImpl(Scaleform::Render::BlendMode, bool, bool)
    ?applyBlendModeImpl@HAL@D3D1x@Render@Scaleform@@UAEXW4BlendMode@34@_N1@Z
0x5b91a0 size 0x169   void Scaleform::Render::D3D1x::HAL::clearSolidRectangle(Scaleform::Render::Rect<int> const&, Scaleform::Render::Color)
    ?clearSolidRectangle@HAL@D3D1x@Render@Scaleform@@UAEXABV?$Rect@H@34@VColor@34@@Z
0x5b9310 size 0x17e   void Scaleform::Render::D3D1x::HAL::PopMask()
    ?PopMask@HAL@D3D1x@Render@Scaleform@@UAEXXZ
0x5b9490 size 0x104   void Scaleform::Render::D3D1x::HAL::EndMaskSubmit()
    ?EndMaskSubmit@HAL@D3D1x@Render@Scaleform@@UAEXXZ
0x5b95a0 size 0x702   void Scaleform::Render::D3D1x::HAL::DrawProcessedComplexMeshes(Scaleform::Render::ComplexMesh*, Scaleform::Render::StrideArray<Scaleform::Render::MatrixPoolImpl::HMatrix> const&)
    ?DrawProcessedComplexMeshes@HAL@D3D1x@Render@Scaleform@@UAEXPAVComplexMesh@34@ABV?$StrideArray@VHMatrix@MatrixPoolImpl@Render@Scaleform@@@34@@Z
0x5b9cb0 size 0x45    Scaleform::Render::PrimitiveFill* Scaleform::Render::D3D1x::HAL::CreatePrimitiveFill(Scaleform::Render::PrimitiveFillData const&)
    ?CreatePrimitiveFill@HAL@D3D1x@Render@Scaleform@@UAEPAVPrimitiveFill@34@ABUPrimitiveFillData@34@@Z
0x5b9d00 size 0x5c    void Scaleform::Render::D3D1x::HAL::beginDisplay(Scaleform::Render::BeginDisplayData*)
    ?beginDisplay@HAL@D3D1x@Render@Scaleform@@UAEXPAVBeginDisplayData@34@@Z
0x5b9d60 size 0xaa    bool Scaleform::Render::D3D1x::HAL::EndScene()
    ?EndScene@HAL@D3D1x@Render@Scaleform@@UAE_NXZ
0x5b9e10 size 0xde    bool Scaleform::Render::D3D1x::HAL::BeginScene()
    ?BeginScene@HAL@D3D1x@Render@Scaleform@@UAE_NXZ
0x5b9ef0 size 0x61    void Scaleform::Render::D3D1x::HAL::~HAL()
    ??1HAL@D3D1x@Render@Scaleform@@UAE@XZ
0x5b9f60 size 0x55e   void Scaleform::Render::D3D1x::HAL::PushMask_BeginSubmit(Scaleform::Render::MaskPrimitive*)
    ?PushMask_BeginSubmit@HAL@D3D1x@Render@Scaleform@@UAEXPAVMaskPrimitive@34@@Z
0x5ba4c0 size 0x291   void Scaleform::Render::D3D1x::HAL::DrawProcessedPrimitive(Scaleform::Render::Primitive*, Scaleform::Render::PrimitiveBatch*, Scaleform::Render::PrimitiveBatch*)
    ?DrawProcessedPrimitive@HAL@D3D1x@Render@Scaleform@@UAEXPAVPrimitive@34@PAVPrimitiveBatch@34@1@Z
0x5ba760 size 0xd0    Scaleform::Render::D3D1x::HAL::HAL(Scaleform::Render::ThreadCommandQueue*)
    ??0HAL@D3D1x@Render@Scaleform@@QAE@PAVThreadCommandQueue@23@@Z
0x5ba830 size 0x28a   bool Scaleform::Render::D3D1x::HAL::InitHAL(Scaleform::Render::D3D1x::HALInitParams const&)
    ?InitHAL@HAL@D3D1x@Render@Scaleform@@UAE_NABUHALInitParams@234@@Z
0x7f0ac0 size 0xb     void `Scaleform::Render::HAL::GetEvent'::`2'::`dynamic atexit destructor for 'defaultEvent''()
    ??__FdefaultEvent@?1??GetEvent@HAL@Render@Scaleform@@UAEAAVRenderEvent@23@W4EventType@23@@Z@YAXXZ
