# vostok/scaleform/sources/d3d1x_meshcache.h - compiland MISSING from binaries/structure/target (parser gap)
#
# 22 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x0427c0 size 0x3     Scaleform::Render::D3D1x::MeshBuffer::BufferType Scaleform::Render::D3D1x::VertexBuffer::GetBufferType() const
    ?get_h_scroll@ui_scroll_view@ui@vostok@@UAEPAUscroll_source@23@XZ
0x0b48e0 size 0x4a    static Scaleform::Render::D3D1x::MeshCacheItem* Scaleform::Render::D3D1x::MeshCacheItem::Create(Scaleform::Render::MeshCacheItem::MeshType, Scaleform::Render::MeshCacheListSet*, Scaleform::Render::MeshCacheItem::MeshBaseContent&, Scaleform::Render::D3D1x::VertexBuffer*, Scaleform::Render::D3D1x::IndexBuffer*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)
    ?Create@MeshCacheItem@D3D1x@Render@Scaleform@@SAPAV1234@W4MeshType@134@PAVMeshCacheListSet@34@AAUMeshBaseContent@134@PAVVertexBuffer@234@PAVIndexBuffer@234@IIIIII@Z
0x0b4930 size 0x6b    void Scaleform::Render::D3D1x::MeshBufferSet::DestroyBuffers(Scaleform::Render::MeshBuffer::AllocType)
    ?DestroyBuffers@MeshBufferSet@D3D1x@Render@Scaleform@@QAEXW4AllocType@MeshBuffer@34@@Z
0x0b49a0 size 0x38    void* Scaleform::Render::D3D1x::MeshBufferSet::`scalar deleting destructor'(unsigned int)
    ??_G?$MeshBufferSetImpl@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b49a0 size 0x38    void* Scaleform::Render::D3D1x::MeshBufferSetImpl<Scaleform::Render::D3D1x::IndexBuffer>::`scalar deleting destructor'(unsigned int)
    ??_G?$MeshBufferSetImpl@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b49a0 size 0x38    void* Scaleform::Render::D3D1x::MeshBufferSetImpl<Scaleform::Render::D3D1x::VertexBuffer>::`scalar deleting destructor'(unsigned int)
    ??_G?$MeshBufferSetImpl@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b49e0 size 0x53    bool Scaleform::Render::D3D1x::VertexBuffer::allocBuffer(ID3D11Device*)
    ?allocBuffer@VertexBuffer@D3D1x@Render@Scaleform@@UAE_NPAUID3D11Device@@@Z
0x0b4a40 size 0x33    void* Scaleform::Render::D3D1x::IndexBuffer::`scalar deleting destructor'(unsigned int)
    ??_GVertexBuffer@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b4a40 size 0x33    void* Scaleform::Render::D3D1x::VertexBuffer::`scalar deleting destructor'(unsigned int)
    ??_GVertexBuffer@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b4a80 size 0x50    bool Scaleform::Render::D3D1x::IndexBuffer::allocBuffer(ID3D11Device*)
    ?allocBuffer@IndexBuffer@D3D1x@Render@Scaleform@@UAE_NPAUID3D11Device@@@Z
0x0b4ad0 size 0x7     bool Scaleform::Render::D3D1x::MeshCache::AreBuffersLocked() const
    ?AreBuffersLocked@MeshCache@D3D1x@Render@Scaleform@@UBE_NXZ
0x0b4ae0 size 0x1e    void* Scaleform::Render::D3D1x::MeshCache::`scalar deleting destructor'(unsigned int)
    ??_GMeshCache@D3D1x@Render@Scaleform@@UAEPAXI@Z
0x0b4b00 size 0x20    void Scaleform::Render::D3D1x::MeshBufferImpl<ID3D11Buffer,Scaleform::Render::D3D1x::IndexBuffer>::Unlock(ID3D11DeviceContext*)
    ?Unlock@?$MeshBufferImpl@UID3D11Buffer@@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAEXPAUID3D11DeviceContext@@@Z
0x0b4b00 size 0x20    void Scaleform::Render::D3D1x::MeshBufferImpl<ID3D11Buffer,Scaleform::Render::D3D1x::VertexBuffer>::Unlock(ID3D11DeviceContext*)
    ?Unlock@?$MeshBufferImpl@UID3D11Buffer@@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAEXPAUID3D11DeviceContext@@@Z
0x0b4b20 size 0x35    bool Scaleform::Render::D3D1x::MeshBufferImpl<ID3D11Buffer,Scaleform::Render::D3D1x::IndexBuffer>::DoLock(ID3D11DeviceContext*)
    ?DoLock@?$MeshBufferImpl@UID3D11Buffer@@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAE_NPAUID3D11DeviceContext@@@Z
0x0b4b20 size 0x35    bool Scaleform::Render::D3D1x::MeshBufferImpl<ID3D11Buffer,Scaleform::Render::D3D1x::VertexBuffer>::DoLock(ID3D11DeviceContext*)
    ?DoLock@?$MeshBufferImpl@UID3D11Buffer@@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAE_NPAUID3D11DeviceContext@@@Z
0x0b4b60 size 0x25    Scaleform::Render::D3D1x::MeshBuffer* Scaleform::Render::D3D1x::MeshBufferSetImpl<Scaleform::Render::D3D1x::VertexBuffer>::CreateBuffer(unsigned int, Scaleform::Render::MeshBuffer::AllocType, unsigned int, Scaleform::MemoryHeap*, ID3D11Device*)
    ?CreateBuffer@?$MeshBufferSetImpl@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAEPAVMeshBuffer@234@IW4AllocType@534@IPAVMemoryHeap@4@PAUID3D11Device@@@Z
0x0b4b90 size 0x25    Scaleform::Render::D3D1x::MeshBuffer* Scaleform::Render::D3D1x::MeshBufferSetImpl<Scaleform::Render::D3D1x::IndexBuffer>::CreateBuffer(unsigned int, Scaleform::Render::MeshBuffer::AllocType, unsigned int, Scaleform::MemoryHeap*, ID3D11Device*)
    ?CreateBuffer@?$MeshBufferSetImpl@VIndexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@UAEPAVMeshBuffer@234@IW4AllocType@534@IPAVMemoryHeap@4@PAUID3D11Device@@@Z
0x0b4bc0 size 0xf7    static Scaleform::Render::D3D1x::VertexBuffer* Scaleform::Render::D3D1x::MeshBufferImpl<ID3D11Buffer,Scaleform::Render::D3D1x::VertexBuffer>::Create(unsigned int, Scaleform::Render::MeshBuffer::AllocType, unsigned int, Scaleform::MemoryHeap*, ID3D11Device*, Scaleform::Render::D3D1x::MeshBufferSet&)
    ?Create@?$MeshBufferImpl@UID3D11Buffer@@VVertexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@SAPAVVertexBuffer@234@IW4AllocType@MeshBuffer@34@IPAVMemoryHeap@4@PAUID3D11Device@@AAVMeshBufferSet@234@@Z
0x0b4cc0 size 0xf7    static Scaleform::Render::D3D1x::IndexBuffer* Scaleform::Render::D3D1x::MeshBufferImpl<ID3D11Buffer,Scaleform::Render::D3D1x::IndexBuffer>::Create(unsigned int, Scaleform::Render::MeshBuffer::AllocType, unsigned int, Scaleform::MemoryHeap*, ID3D11Device*, Scaleform::Render::D3D1x::MeshBufferSet&)
    ?Create@?$MeshBufferImpl@UID3D11Buffer@@VIndexBuffer@D3D1x@Render@Scaleform@@@D3D1x@Render@Scaleform@@SAPAVIndexBuffer@234@IW4AllocType@MeshBuffer@34@IPAVMemoryHeap@4@PAUID3D11Device@@AAVMeshBufferSet@234@@Z
0x0b53b0 size 0x6     Scaleform::Render::D3D1x::MeshBuffer::BufferType Scaleform::Render::D3D1x::IndexBuffer::GetBufferType() const
    ?GetDrawableImageFormat@TextureManager@Render@Scaleform@@UBE?AW4ImageFormat@23@XZ
0x0b53b0 size 0x6     Scaleform::Render::MeshCache::QueueMode Scaleform::Render::D3D1x::MeshCache::GetQueueMode() const
    ?GetDrawableImageFormat@TextureManager@Render@Scaleform@@UBE?AW4ImageFormat@23@XZ
