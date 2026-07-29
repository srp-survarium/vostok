////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< D3D11_INPUT_ELEMENT_DESC >::rebind< D3D11_INPUT_ELEMENT_DESC > {
}; // struct std_allocator< D3D11_INPUT_ELEMENT_DESC >::rebind< D3D11_INPUT_ELEMENT_DESC >

STATIC_SIZE_ASSERT(std_allocator< D3D11_INPUT_ELEMENT_DESC >::rebind< D3D11_INPUT_ELEMENT_DESC >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
