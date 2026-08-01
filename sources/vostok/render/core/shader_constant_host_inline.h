#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_HOST_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_HOST_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline shader_constant_host::shader_constant_host( shared_string const& name, enum_constant_type type ) :
	m_source( 0, 0 ),
	m_name( name ),
	m_type( type )
{
	reset_update_markers( );
}

inline bool operator==( shader_constant_host const& left, shared_string const& right )
{
	return left.name( ) == right;
}

inline bool operator==( shared_string const& left, shader_constant_host const& right )
{
	return left == right.name( );
}

inline bool operator<( shader_constant_host const& left, shared_string const& right )
{
	return left.name( ) < right;
}

inline bool operator<( shared_string const& left, shader_constant_host const& right )
{
	return left < right.name( );
}

inline bool operator==( shader_constant_host const& left, shader_constant_host const& right )
{
	return left.name( ) == right.name( );
}

inline bool operator<( shader_constant_host const& left, shader_constant_host const& right )
{
	return left.name( ) < right.name( );
}

inline s32 compare( shader_constant_host const& left, shader_constant_host const& right )
{
	if ( left.name( ).c_str( ) < right.name( ).c_str( ) )
		return -1;

	return right.name( ).c_str( ) < left.name( ).c_str( ) ? 1 : 0;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_HOST_INLINE_H_INCLUDED
