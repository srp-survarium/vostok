////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record >::rebind< state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record > {
}; // struct std_allocator< state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record >::rebind< state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record >

STATIC_SIZE_ASSERT(std_allocator< state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record >::rebind< state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
