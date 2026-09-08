<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Scaleform reference-header ownership

The 57 literal no-source markers in `vostok/scaleform/sources` are in three
PDB reference mirrors, not the headers consumed by the implementation units:
`d3d1x_hal.h` (16), `d3d1x_meshcache.h` (35), and `d3d1x_shaderdescs.h` (6).
Fourteen additional distinct `STATE[STUB]` declarations in the same mirrors are
also accounted for below; four other STUB tags overlap the literal markers.

The include graph establishes the distinction. `d3d1x_hal.cpp` and
`d3d1x_meshcache.cpp` include `Render/D3D1x/D3D1x_HAL.h` and
`Render/D3D1x/D3D1x_MeshCache.h`; the generated shader source likewise includes
the SDK's `D3D1x_ShaderDescs.h`. The project include roots select
`sources/scaleform/Src`. The only local-mirror mesh-cache include comes from
the unused local HAL mirror, not an active translation unit. A vcproj header
listing is not an include edge. The generated ninja graph also contains the
actual implementation translation units; they are not all excluded from builds.

Do not duplicate these implementations in the mirrors or edit the fenced SDK
to satisfy this marker census. There is available, consumed source. This
classification does **not** claim that all these functions are already
byte-exact; the old mirror comments' blanket claim was removed.

Source keys, all within this repository:

- **H**: `sources/scaleform/Src/Render/D3D1x/D3D1x_HAL.h`.
- **M**: `sources/scaleform/Src/Render/D3D1x/D3D1x_MeshCache.h`.
- **E**: `sources/scaleform/Src/Render/D3D1x/D3D1x_Events.h`.
- **HC**: `sources/vostok/scaleform/sources/d3d1x_hal.cpp`.
- **D**: `sources/vostok/scaleform/sources/d3d1x_shaderdescs.cpp`, whose
  declarations come from the SDK header rather than the local mirror.

The flake-fetched 4.2.22 SDK is also present in the Nix store and supplies a
source prior. The mappings below use the in-tree consumed sources, rather than
assuming that a nearby SDK version or sibling checkout is the active input.

## HAL mirror: all 16 literal-marker declarations

| Function | Source owner and semantics |
|---|---|
| `HALInitParams::HALInitParams` | H: initialize the base flags/thread record and device/context fields. |
| `HALInitParams::SetTextureManager` | H: assign the base smart pointer. |
| `HALInitParams::GetTextureManager` | H: get the smart pointer and cast to the D3D manager type. |
| `HALInitParams::~HALInitParams` | H: implicit destruction, including the base-owned smart pointer; not an invented empty resource destructor. |
| `RenderTargetData::~RenderTargetData` | H: release each non-null render/depth view, followed by base destruction. |
| `RenderEvent::Begin` | E: PIX/annotation event setup in non-shipping configurations; compiled out in shipping. |
| `RenderEvent::End` | E: matching PIX/annotation end in non-shipping configurations. |
| `RenderEvent::RenderEvent` | E: implicit constructor; no per-instance context ownership. |
| `RenderEvent::~RenderEvent` | E: implicit destructor; global context release belongs to `ShutdownEvents`. |
| `RenderEvent::InitializeEvents` | E: save/AddRef the context; optional non-shipping PIX API loading. |
| `RenderEvent::ShutdownEvents` | E: release and clear the saved context. It is not an empty shipping hook. |
| `HAL::getThis` | H: return this. |
| `HAL::PrepareForReset` | HC: skip repeated preparation, release default render/depth views, notify handlers, set ready-for-reset state. The real definition is out-of-line. |
| `HAL::SetRasterMode` | H: assign raster mode. |
| `HAL::GetDevice` | H: return the device pointer. |
| `HAL::GetBlendType` | H: choose ordinary/source-alpha/no-color-write blend-table region and add the blend mode. |

## Mesh-cache mirror: all 35 literal-marker declarations

All source owners in this table are **M**. Template destructors without an
explicit SDK body are implicit C++ destruction, not missing custom source.

| Function | Existing source semantics |
|---|---|
| `MeshBuffer::LockList::LockList` | Initialize the list head to null. |
| `MeshBuffer::LockList::Add` | Prepend via the buffer's next-lock field. |
| `MeshBuffer::LockList::UnlockAll` | Unlock every recorded buffer and clear the head. |
| `MeshBuffer::MeshBuffer` | Delegate size/type/arena initialization to the render base. |
| `MeshBuffer::~MeshBuffer` | Explicit empty derived body plus base destruction. |
| `MeshBuffer::GetIndex` | Return the buffer-set index. |
| `MeshBuffer::Lock` | Lock only if no data is mapped; register successful locks and return the mapped data. |
| `MeshBufferSet::MeshBufferSet` | Construct allocator, store granularity, initialize total size to zero. |
| `MeshBufferSet::~MeshBufferSet` | Explicit empty body, followed by member destruction. |
| `MeshBufferSet::SetGranularity` | Store granularity. |
| `MeshBufferSet::GetAllocator` | Return allocator reference. |
| `MeshBufferSet::GetGranularity` | Return granularity. |
| `MeshBufferSet::GetTotalSize` | Return total allocation bytes. |
| `MeshBufferSet::DestroyBuffer` | Remove allocator segment, subtract size, clear the indexed slot; optionally delete buffer. |
| `MeshBufferSet::Alloc` | Allocate rounded units, decode high index bits and low offset bits; fail on allocator sentinel. |
| `MeshBufferSet::Free` | Encode index/offset, free rounded units and convert the returned size to bytes. |
| `MeshBufferSet::SizeToAllocatorUnit` | Round byte size upward to 16-byte units. |
| `MeshBufferSetImpl::MeshBufferSetImpl` | Delegate to the buffer-set base. |
| `MeshBufferSetImpl::CreateBuffer` | Assert addressable size and invoke the specialized buffer factory. |
| `MeshBufferSetImpl::~MeshBufferSetImpl` | Implicit base destruction. |
| `MeshBufferImpl::MeshBufferImpl` | Delegate to the mesh-buffer base. |
| `MeshBufferImpl::~MeshBufferImpl` | Implicit destruction includes the hardware-buffer smart pointer. |
| `MeshBufferImpl::GetHWBuffer` | Return the hardware smart pointer's raw pointer. |
| `MeshBufferImpl::DoLock` | Map with write-no-overwrite, save mapped data and return mapping success. |
| `MeshBufferImpl::Unlock` | Unmap and clear the data pointer. |
| `MeshBufferImpl::Create` | Find a free index, round size, allocate/create hardware buffer, register the segment and update set accounting; clean up on failure. |
| `VertexBuffer::VertexBuffer` | Delegate size/type/arena to its specialized base. |
| `VertexBuffer::~VertexBuffer` | Implicit base destruction. |
| `IndexBuffer::IndexBuffer` | Delegate size/type/arena to its specialized base. |
| `IndexBuffer::~IndexBuffer` | Implicit base destruction. |
| `MeshCacheItem::MeshCacheItem` | No explicit derived constructor in M; compiler-provided construction, not a separately missing handwritten body. |
| `MeshCacheItem::~MeshCacheItem` | Implicit base destruction; creation policy is in `Create`. |
| `MeshCache::getThis` | Return this. |
| `MeshCache::getTotalSize` | Sum vertex-buffer and index-buffer totals. |
| `MeshCache::GetRenderSync` | Return the member synchronization object. |

## Shader-descriptor mirror: all six literal-marker declarations

All six already have actual out-of-line definitions in **D**.

| Function | Existing source semantics |
|---|---|
| `ShaderDesc::GetShaderTypeForComboIndex` | Version switch returns `ST_None` for the generated configuration's unused combo interface. |
| `VertexShaderDesc::GetShaderComboIndex` | Generated version branches return zero. |
| `VertexShaderDesc::GetShaderIndexForComboIndex` | Generated version branches return `VSI_None`. |
| `FragShaderDesc::GetDesc` | Index the descriptor table using `GetShaderIndex(shader,version)`. |
| `FragShaderDesc::GetShaderComboIndex` | Generated version branches return zero. |
| `FragShaderDesc::GetShaderIndexForComboIndex` | Generated version branches return `FSI_None`. |

The five combo-interface constant returns are real generated source, not
missing lookup-table reconstructions. Do not replace them with guessed mappings
just because the reference mirror also contains a zero return.

## Fourteen additional distinct STUB-tagged declarations

| Function | Existing source owner/semantics |
|---|---|
| `RenderTargetData::RenderTargetData` | H: initialize base/views and AddRef both non-null views. |
| `RenderTargetData::UpdateData` | H: allocate data if absent; otherwise retain new views before releasing and replacing old views. |
| `HAL::beginMaskDisplay` | H: assert empty mask stack, clear capability flags and drawing-mask state. |
| `HAL::GetTextureManager` | H: return manager smart pointer. |
| `HAL::GetMeshCache` | H: return cache reference. |
| `HAL::MapVertexFormat` | H: forward format outputs to `SManager`; extra flags parameter is unused. |
| `MeshBufferSet::DestroyBuffers` | M: iterate allocated slots and destroy matching allocation types. |
| `VertexBuffer::allocBuffer` | M: create dynamic CPU-writable vertex buffer with the recorded descriptor fields. |
| `VertexBuffer::GetBufferType` | M: vertex-buffer enum. |
| `IndexBuffer::allocBuffer` | M: same construction with index-buffer bind flags. |
| `IndexBuffer::GetBufferType` | M: index-buffer enum. |
| `MeshCacheItem::Create` | M: base allocation followed by buffer pointers and vertex/index allocation metadata. |
| `MeshCache::GetQueueMode` | M: extend-locks queue mode. |
| `MeshCache::AreBuffersLocked` | M: return locked flag. |

Result: **71 distinct marked declarations have a source owner**, without any
new implementation or SDK edit. The three mirror headers remain reference
material; their placeholders must not be counted as active missing bodies by
a subsequent inventory. Their declaration-only/out-of-line members likewise
belong to the actual SDK headers and implementation units, not to this mirror.
No build or ledger refresh was run.
