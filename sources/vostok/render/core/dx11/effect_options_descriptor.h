#ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_OPTIONS_DESCRIPTOR_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_EFFECT_OPTIONS_DESCRIPTOR_H_INCLUDED

#include <boost/noncopyable.hpp>

#include <vostok/configs.h>
#include <vostok/memory_extensions.h>
#include <vostok/strings_functions.h>
#include <vostok/render/core/destroy_data_helper.h>
#include <vostok/render/core/static_type.h>

namespace vostok {
namespace render {

struct effect_options_descriptor : public boost::noncopyable {
	struct const_iterator;

	effect_options_descriptor( );
	effect_options_descriptor( pcvoid in_data, u16 in_size );
	~effect_options_descriptor( );

	u32 get_crc( ) const;
	u32 get_data_memory_usage( u32& need_bytes_to_align4, u32& last_align_value ) const;
	u32 get_num_total_fields( ) const;
	u32 get_num_used_bytes( ) const;
	effect_options_descriptor& operator[]( pcstr key );

	void call_destructors( )
	{
		if ( destroyer && data ) {
			if ( count <= sizeof( u32 ) )
				reinterpret_cast<destroy_data_helper_base*>( &destroyer )->destroy( &data );
			else
				reinterpret_cast<destroy_data_helper_base*>( const_cast<pvoid>( destroyer ) )->destroy( data );
		}

		if ( type == configs::t_table_named ) {
			effect_options_descriptor* child = reinterpret_cast<effect_options_descriptor*>( data );
			for ( u32 i = 0; child && i < count; ++i ) {
				child->call_destructors( );
				child = reinterpret_cast<effect_options_descriptor*>(
					reinterpret_cast<pbyte>( child ) + child->get_num_used_bytes( )
				);
			}
		}
	}

	template < typename T >
	effect_options_descriptor& operator=( T value )
	{
		// Observed instantiations: s32 [0x7ed00] and
		// enum_vertex_input_type [0x63740].
		u16 const type_size = sizeof( T );
		if ( type_size <= sizeof( u32 ) ) {
			memory::zero( &data, sizeof( data ) );
			T& stored_value = *new( &data ) T;
			stored_value = value;
			count = sizeof( u32 );
		}
		else {
			T& stored_value = *new( data ) T;
			stored_value = value;
			if ( type == configs::t_table_named )
				bytes += type_size;
			count = type_size;
		}

		type = static_type::get_type_id<T>( );
		new( &destroyer ) destroy_data_helper<T const>;
		return *this;
	}

	const_iterator begin( ) const;
	const_iterator end( ) const;

	pcstr	id;
	pcvoid	destroyer;
	pbyte	data;
	u32		bytes;
	u16		type;
	u16		count;
	u16		memory_size;
};

struct effect_options_descriptor::const_iterator {
	const_iterator( ) : value( 0 ), child_index( 0 ), size( 0 ) { }

	explicit const_iterator( effect_options_descriptor const* const descriptor ) :
		value( descriptor ? reinterpret_cast<effect_options_descriptor*>( descriptor->data ) : 0 ),
		child_index( 0 ),
		size( descriptor && descriptor->type == configs::t_table_named ? descriptor->count : 0 )
	{
	}

	operator bool( ) { return child_index < size; }

	void operator++( )
	{
		++child_index;
		value = reinterpret_cast<effect_options_descriptor*>(
			reinterpret_cast<pbyte>( value ) + value->get_num_used_bytes( )
		);
	}

	effect_options_descriptor const& operator*( ) { return *value; }
	effect_options_descriptor const* operator->( ) { return value; }
	bool operator!=( const_iterator const& iterator ) { return child_index < iterator.size; }

private:
	effect_options_descriptor*	value;
	u32						child_index;
	u32						size;
};

inline effect_options_descriptor::const_iterator effect_options_descriptor::begin( ) const
{
	return const_iterator( this );
}

inline effect_options_descriptor::const_iterator effect_options_descriptor::end( ) const
{
	return const_iterator( this );
}

template <>
inline effect_options_descriptor& effect_options_descriptor::operator=<pcstr>( pcstr value )
{
	count = u16( strings::length( value ) + 1 );
	memory::copy( data, count, value, count );

	if ( type == configs::t_table_named ) bytes += count;

	type = static_type::get_type_id<pcstr>( );

	return *this;
}

STATIC_SIZE_ASSERT( effect_options_descriptor, 0x18 );
STATIC_SIZE_ASSERT( effect_options_descriptor::const_iterator, 0xC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_OPTIONS_DESCRIPTOR_H_INCLUDED
