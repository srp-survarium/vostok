////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BASE_BASIS_H_INCLUDED
#define RENDER_BASE_BASIS_H_INCLUDED

namespace vostok {
namespace render {

class base_basis {
public:
	inline	void	set	( float3 n ) { /* no source */ }

	/* 0x0000 */	u8		x;
	/* 0x0001 */	u8		y;
	/* 0x0002 */	u8		z;
}; // class base_basis

STATIC_SIZE_ASSERT(base_basis, 0x3);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BASE_BASIS_H_INCLUDED
