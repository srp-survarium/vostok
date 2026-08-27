#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline bool operator==( shader_constant const& left, shader_constant const& right )
{
	return left.host( ) == right.host( )
		&& left.source( ) == right.source( )
		&& left.slot( ) == right.slot( );
}

inline bool operator!=( shader_constant const& left, shader_constant const& right )
{
	return !(left == right);
}

inline s32 compare( shader_constant const& left, shader_constant const& right )
{
	s32 const result = compare( left.host( ), right.host( ) );
	if ( result )
		return result;

	return left.source( ).pointer( ) < right.source( ).pointer( ) ? -1
		: left.source( ).pointer( ) > right.source( ).pointer( ) ? 1
		: left.source( ).size( ) < right.source( ).size( ) ? -1
		: left.source( ).size( ) > right.source( ).size( ) ? 1
		: left.slot( ).value( ) < right.slot( ).value( ) ? -1
		: left.slot( ).value( ) > right.slot( ).value( ) ? 1 : 0;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_INLINE_H_INCLUDED
