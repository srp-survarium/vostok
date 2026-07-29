////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_SOURCE_H_INCLUDED
#define RENDER_SHADER_CONSTANT_SOURCE_H_INCLUDED

namespace vostok {
namespace render {

class shader_constant_source {
public:
	inline				shader_constant_source	( void* arg_0, u32 arg_1 ) { /* no source */ }

	inline	void*		pointer					( ) const { /* no source */ }

	inline	u32			size					( ) const { /* no source */ }

private:
	/* 0x0000 */	void* const		m_pointer;
	/* 0x0004 */	const u32		m_size;
}; // class shader_constant_source

STATIC_SIZE_ASSERT(shader_constant_source, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_SOURCE_H_INCLUDED
