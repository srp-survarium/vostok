# vostok/scaleform/sources/d3d1x_sync.h - compiland MISSING from binaries/structure/target (parser gap)
#
# 10 addressed functions from binaries/rich/target/index.jsonl; use
# pdb_fetch --view carcass/structure per function when reproducing.
# Queue protocol: delete this file when the compiland is reproduced.

0x022c50 size 0x3     void Scaleform::Render::D3D1x::RenderSync::KickOffFences(Scaleform::Render::FenceType)
    ?visit@time_scale_transition_debug@mixing@animation@vostok@@EAEXAAVn_ary_tree_time_scale_node@234@@Z
0x022c50 size 0x3     void Scaleform::Render::MeshCache::PostUpdateMesh(Scaleform::Render::MeshCacheItem*)
    ?visit@time_scale_transition_debug@mixing@animation@vostok@@EAEXAAVn_ary_tree_time_scale_node@234@@Z
0x0b4600 size 0xc9    bool Scaleform::Render::D3D1x::RenderSync::SetDevice(ID3D11Device*, ID3D11DeviceContext*)
    ?SetDevice@RenderSync@D3D1x@Render@Scaleform@@QAE_NPAUID3D11Device@@PAUID3D11DeviceContext@@@Z
0x0b46d0 size 0x48    void Scaleform::Render::D3D1x::RenderSync::BeginFrame()
    ?BeginFrame@RenderSync@D3D1x@Render@Scaleform@@UAEXXZ
0x0b4720 size 0x43    bool Scaleform::Render::D3D1x::RenderSync::EndFrame()
    ?EndFrame@RenderSync@D3D1x@Render@Scaleform@@UAE_NXZ
0x0b4770 size 0x1a    unsigned long long Scaleform::Render::D3D1x::RenderSync::SetFence()
    ?SetFence@RenderSync@D3D1x@Render@Scaleform@@MAE_KXZ
0x0b4790 size 0x3a    bool Scaleform::Render::D3D1x::RenderSync::IsPending(Scaleform::Render::FenceType, unsigned long long, Scaleform::Render::FenceFrame const&)
    ?IsPending@RenderSync@D3D1x@Render@Scaleform@@MAE_NW4FenceType@34@_KABVFenceFrame@34@@Z
0x0b47d0 size 0x95    void Scaleform::Render::D3D1x::RenderSync::WaitFence(Scaleform::Render::FenceType, unsigned long long, Scaleform::Render::FenceFrame const&)
    ?WaitFence@RenderSync@D3D1x@Render@Scaleform@@MAEXW4FenceType@34@_KABVFenceFrame@34@@Z
0x0b4870 size 0x13    void Scaleform::Render::D3D1x::RenderSync::ReleaseFence(unsigned long long)
    ?ReleaseFence@RenderSync@D3D1x@Render@Scaleform@@MAEX_K@Z
0x0b4890 size 0x43    void* Scaleform::Render::D3D1x::RenderSync::`scalar deleting destructor'(unsigned int)
    ??_GRenderSync@D3D1x@Render@Scaleform@@UAEPAXI@Z
