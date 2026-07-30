#ifndef VOSTOK_RENDER_CORE_CUSTOM_CONFIG_VALUE_H_INCLUDED
#define VOSTOK_RENDER_CORE_CUSTOM_CONFIG_VALUE_H_INCLUDED

#include <vostok/configs.h>
#include <vostok/strings_functions.h>
#include <vostok/render/core/static_type.h>

namespace vostok {
namespace render {

bool per_bytes_equal( u8 const* left, u8 const* right, u32 count );

class custom_config_value {
public:
	typedef custom_config_value const* const_iterator;

	custom_config_value( ) :
		id			( 0 ),
		data		( 0 ),
		id_crc		( 0 ),
		type		( 0 ),
		count		( 0 ),
		destroyer	( 0 )
	{
	}

	u32 size( ) const
	{
		return count;
	}

	const_iterator begin( ) const
	{
		R_ASSERT( type == configs::t_table_named );
		return reinterpret_cast<const_iterator>( data );
	}

	const_iterator end( ) const
	{
		R_ASSERT( type == configs::t_table_named );
		return reinterpret_cast<const_iterator>( data ) + count;
	}

	custom_config_value const& operator[]( pcstr key ) const;

	bool operator<( u32 crc ) const
	{
		return id_crc < crc;
	}

	pcvoid get_data_ptr( ) const
	{
		if ( count <= sizeof( u32 ) || type == static_type::get_type_id<pcstr>( ) )
			return &data;
		return data;
	}

	bool value_exists( pcstr key ) const;

	template < typename T >
	operator T( ) const
	{
		// Observed instantiation: math::float3 [0x682a0].
		ASSERT_CMP( static_type::get_type_id<T>( ), ==, type );
		if ( identity( sizeof( T ) <= sizeof( u32 ) )
			|| type == static_type::get_type_id<pcstr>( ) )
			return *reinterpret_cast<T const*>( &data );
		return *reinterpret_cast<T const*>( data );
	}

	template < typename config_value_type >
	bool operator==( config_value_type const& right ) const
	{
		custom_config_value const& left = *this;
		if ( left.id_crc != right.id_crc || left.count != right.count )
			return false;

		if ( left.type == configs::t_table_named
			&& (right.type == configs::t_table_named
				|| right.type == configs::t_table_indexed) ) {
			const_iterator left_iterator = left.begin( );
			const_iterator const left_end = left.end( );
			typename config_value_type::const_iterator right_iterator = right.begin( );
			for ( ; left_iterator != left_end; ++left_iterator, ++right_iterator )
				if ( !( *left_iterator == *right_iterator ) )
					return false;
		}
		else if ( left.type == static_type::get_type_id<pcstr>( ) ) {
			if ( strings::compare( pcstr( left.data ), pcstr( pcvoid( right.data ) ) ) != 0 )
				return false;
		}
		else if ( left.count <= sizeof( u32 ) ) {
			if ( left.data != right.data )
				return false;
		}
		else if ( !per_bytes_equal(
			reinterpret_cast<u8 const*>( left.data ),
			reinterpret_cast<u8 const*>( pcvoid( right.data ) ),
			left.count
		) )
			return false;

		return true;
	}

private:
	friend struct custom_config;
	void call_data_destructor( ) const;

public:
	pcstr	id;
	pcvoid	data;
	u32		id_crc;
	u16		type;
	u16		count;
	pcvoid	destroyer;
};

template <>
inline custom_config_value::operator float( ) const
{
	bool const is_integer_type =
		type == static_type::get_type_id<u8>( ) || type == static_type::get_type_id<s8>( )
		|| type == static_type::get_type_id<u16>( ) || type == static_type::get_type_id<s16>( )
		|| type == static_type::get_type_id<u32>( ) || type == static_type::get_type_id<s32>( )
		|| type == static_type::get_type_id<u64>( ) || type == static_type::get_type_id<s64>( );

	if ( is_integer_type )
		return static_cast<float>( reinterpret_cast<size_t>( data ) );

	ASSERT_CMP( static_type::get_type_id<float>( ), ==, type );
	if ( identity( sizeof( float ) <= sizeof( u32 ) ) )
		return *reinterpret_cast<float const*>( &data );
	return *reinterpret_cast<float const*>( data );
}

template <>
inline custom_config_value::operator double( ) const
{
	bool const is_integer_type =
		type == static_type::get_type_id<u8>( ) || type == static_type::get_type_id<s8>( )
		|| type == static_type::get_type_id<u16>( ) || type == static_type::get_type_id<s16>( )
		|| type == static_type::get_type_id<u32>( ) || type == static_type::get_type_id<s32>( )
		|| type == static_type::get_type_id<u64>( ) || type == static_type::get_type_id<s64>( );

	if ( is_integer_type )
		return static_cast<double>( reinterpret_cast<size_t>( data ) );

	ASSERT_CMP( static_type::get_type_id<double>( ), ==, type );
	return *reinterpret_cast<double const*>( data );
}

STATIC_SIZE_ASSERT( custom_config_value, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_CUSTOM_CONFIG_VALUE_H_INCLUDED
