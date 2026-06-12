# vostok/scaleform/sources/d3d1x_meshcache.cpp - compiland MISSING from binaries/structure/target (parser gap)
#
# 22 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x5b4eb0 size 0x20    bool Scaleform::Render::D3D1x::MeshCache::LockBuffers()
    ?LockBuffers@MeshCache@D3D1x@Render@Scaleform@@UAE_NXZ
0x5b4ed0 size 0x76    bool Scaleform::Render::D3D1x::MeshCache::createMaskEraseBatchVertexBuffer(ID3D11Device*)
    ?createMaskEraseBatchVertexBuffer@MeshCache@D3D1x@Render@Scaleform@@AAE_NPAUID3D11Device@@@Z
0x5b4f50 size 0x7     bool Scaleform::Render::D3D1x::MeshCache::createStaticVertexBuffers(ID3D11Device*)
    ?createStaticVertexBuffers@MeshCache@D3D1x@Render@Scaleform@@AAE_NPAUID3D11Device@@@Z
0x5b4f60 size 0x11    void Scaleform::Render::D3D1x::MeshCache::BeginFrame()
    ?BeginFrame@MeshCache@D3D1x@Render@Scaleform@@UAEXXZ
0x5b4f80 size 0x98    void Scaleform::Render::D3D1x::MeshCache::GetStats(Scaleform::Render::MeshCache::Stats*)
    ?GetStats@MeshCache@D3D1x@Render@Scaleform@@UAEXPAUStats@134@@Z
0x5b5020 size 0x43    void Scaleform::Render::D3D1x::MeshCache::UnlockBuffers()
    ?UnlockBuffers@MeshCache@D3D1x@Render@Scaleform@@UAEXXZ
0x5b5070 size 0x118   void Scaleform::Render::D3D1x::MeshCache::destroyPendingBuffers()
    ?destroyPendingBuffers@MeshCache@D3D1x@Render@Scaleform@@AAEXXZ
0x5b5190 size 0x50    void Scaleform::Render::D3D1x::MeshCache::adjustMeshCacheParams(Scaleform::Render::MeshCacheParams*)
    ?adjustMeshCacheParams@MeshCache@D3D1x@Render@Scaleform@@AAEXPAUMeshCacheParams@34@@Z
0x5b51e0 size 0x1cd   bool Scaleform::Render::D3D1x::MeshCache::PreparePrimitive(Scaleform::Render::PrimitiveBatch*, Scaleform::Render::MeshCacheItem::MeshContent&, bool)
    ?PreparePrimitive@MeshCache@D3D1x@Render@Scaleform@@UAE_NPAVPrimitiveBatch@34@AAUMeshContent@MeshCacheItem@34@_N@Z
0x5b53b0 size 0x205   bool Scaleform::Render::D3D1x::MeshCache::allocBuffer(unsigned int*, Scaleform::Render::D3D1x::MeshBuffer**, Scaleform::Render::D3D1x::MeshBufferSet&, unsigned int, bool)
    ?allocBuffer@MeshCache@D3D1x@Render@Scaleform@@AAE_NPAIPAPAVMeshBuffer@234@AAVMeshBufferSet@234@I_N@Z
0x5b55c0 size 0x101   unsigned int Scaleform::Render::D3D1x::MeshCache::Evict(Scaleform::Render::MeshCacheItem*, Scaleform::AllocAddr*, Scaleform::Render::MeshBase*)
    ?Evict@MeshCache@D3D1x@Render@Scaleform@@UAEIPAVMeshCacheItem@34@PAVAllocAddr@4@PAVMeshBase@34@@Z
0x5b56d0 size 0xe2    bool Scaleform::Render::D3D1x::MeshCache::evictMeshesInBuffer(Scaleform::Render::MeshCacheListSet::ListSlot*, unsigned int, Scaleform::Render::D3D1x::MeshBuffer*)
    ?evictMeshesInBuffer@MeshCache@D3D1x@Render@Scaleform@@AAE_NPAUListSlot@MeshCacheListSet@34@IPAVMeshBuffer@234@@Z
0x5b57c0 size 0xb9    bool Scaleform::Render::D3D1x::MeshCache::allocCacheBuffers(unsigned int, Scaleform::Render::MeshBuffer::AllocType, unsigned int)
    ?allocCacheBuffers@MeshCache@D3D1x@Render@Scaleform@@AAE_NIW4AllocType@MeshBuffer@34@I@Z
0x5b5880 size 0x14f   void Scaleform::Render::D3D1x::MeshCache::EndFrame()
    ?EndFrame@MeshCache@D3D1x@Render@Scaleform@@UAEXXZ
0x5b59d0 size 0x47    void Scaleform::Render::D3D1x::MeshCache::destroyBuffers(Scaleform::Render::MeshBuffer::AllocType)
    ?destroyBuffers@MeshCache@D3D1x@Render@Scaleform@@AAEXW4AllocType@MeshBuffer@34@@Z
0x5b5a20 size 0x43    void Scaleform::Render::D3D1x::MeshCache::ClearCache()
    ?ClearCache@MeshCache@D3D1x@Render@Scaleform@@UAEXXZ
0x5b5a70 size 0x8d    void Scaleform::Render::D3D1x::MeshCache::Reset()
    ?Reset@MeshCache@D3D1x@Render@Scaleform@@QAEXXZ
0x5b5b00 size 0xb2    bool Scaleform::Render::D3D1x::MeshCache::Initialize(ID3D11Device*, ID3D11DeviceContext*, Scaleform::Render::D3D1x::ShaderManager*)
    ?Initialize@MeshCache@D3D1x@Render@Scaleform@@QAE_NPAUID3D11Device@@PAUID3D11DeviceContext@@PAVShaderManager@234@@Z
0x5b5bc0 size 0x1f6   Scaleform::Render::MeshCache::AllocResult Scaleform::Render::D3D1x::MeshCache::AllocCacheItem(Scaleform::Render::MeshCacheItem**, unsigned char**, unsigned short**, Scaleform::Render::MeshCacheItem::MeshType, Scaleform::Render::MeshCacheItem::MeshBaseContent&, unsigned int, unsigned int, unsigned int, bool, Scaleform::Render::VertexFormat const*)
    ?AllocCacheItem@MeshCache@D3D1x@Render@Scaleform@@UAE?AW4AllocResult@134@PAPAVMeshCacheItem@34@PAPAEPAPAGW4MeshType@634@AAUMeshBaseContent@634@III_NPBUVertexFormat@34@@Z
0x5b5dc0 size 0x166   bool Scaleform::Render::D3D1x::MeshCache::SetParams(Scaleform::Render::MeshCacheParams const&)
    ?SetParams@MeshCache@D3D1x@Render@Scaleform@@UAE_NABUMeshCacheParams@34@@Z
0x5b5f30 size 0xca    void Scaleform::Render::D3D1x::MeshCache::~MeshCache()
    ??1MeshCache@D3D1x@Render@Scaleform@@UAE@XZ
0x5b6000 size 0x162   Scaleform::Render::D3D1x::MeshCache::MeshCache(Scaleform::MemoryHeap*, Scaleform::Render::MeshCacheParams const&)
    ??0MeshCache@D3D1x@Render@Scaleform@@QAE@PAVMemoryHeap@3@ABUMeshCacheParams@23@@Z
