////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >::rebind< std::pair< fixed_string< 64 >, ID3D11SamplerState* > > {
}; // struct std_allocator< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >::rebind< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >::rebind< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
