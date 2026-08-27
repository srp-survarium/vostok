#ifndef VOSTOK_RENDER_CORE_DX11_CUSTOM_CONFIG_VALUE_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_CUSTOM_CONFIG_VALUE_INLINE_H_INCLUDED

namespace vostok {
namespace render {

template < typename T >
inline custom_config_value::operator T( ) const
{
	ASSERT_CMP( static_type::get_type_id<T>( ), ==, type );
	if ( identity( sizeof( T ) <= sizeof( u32 ) )
		|| type == static_type::get_type_id<pcstr>( ) )
		return *reinterpret_cast<T const*>( &data );
	return *reinterpret_cast<T const*>( data );
}

template <>
inline custom_config_value::operator float( ) const
{
	bool is_integer_type =
		type == static_type::get_type_id<u8>( ) || type == static_type::get_type_id<s8>( )
		|| type == static_type::get_type_id<u16>( ) || type == static_type::get_type_id<s16>( )
		|| type == static_type::get_type_id<u32>( ) || type == static_type::get_type_id<s32>( )
		|| type == static_type::get_type_id<u64>( ) || type == static_type::get_type_id<s64>( );

	if ( is_integer_type )
		return (float)u64( data );

	ASSERT_CMP( static_type::get_type_id<float>( ), ==, type );
	if ( identity( sizeof( float ) <= sizeof( u32 ) ) || type == static_type::get_type_id<pcstr>( ) )
		return *(float*)&data;
	else
		return *(float*)data;
}

template <>
inline custom_config_value::operator double( ) const
{
	bool is_integer_type =
		type == static_type::get_type_id<u8>( ) || type == static_type::get_type_id<s8>( )
		|| type == static_type::get_type_id<u16>( ) || type == static_type::get_type_id<s16>( )
		|| type == static_type::get_type_id<u32>( ) || type == static_type::get_type_id<s32>( )
		|| type == static_type::get_type_id<u64>( ) || type == static_type::get_type_id<s64>( );

	if ( is_integer_type )
		return (double)u64( data );

	ASSERT_CMP( static_type::get_type_id<double>( ), ==, type );
	if ( identity( sizeof( double ) <= sizeof( u32 ) ) || type == static_type::get_type_id<pcstr>( ) )
		return *(double*)&data;
	else
		return *(double*)data;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_CUSTOM_CONFIG_VALUE_INLINE_H_INCLUDED
