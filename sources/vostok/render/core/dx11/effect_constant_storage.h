#ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_CONSTANT_STORAGE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_EFFECT_CONSTANT_STORAGE_H_INCLUDED

#include <algorithm>

#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/shader_constant_defines.h>

namespace vostok {
namespace render {

struct data_indexer {
	data_indexer( u32* data, u32 type ) : data_ptr( data ), class_id( type ) { }

	u32* data_ptr;
	u32 class_id;
};

bool constant_data_predicate( data_indexer const& left, data_indexer const& right );

struct fixed_constants_data_buffer {
	enum { buffer_size = 1024 };

	fixed_constants_data_buffer( ) : next( 0 ), m_pos( 0 ) { }
	bool is_overflowing( u32 bytes ) const { return m_pos + bytes > buffer_size; }

	template < typename T >
	pbyte place( )
	{
		pbyte result = &m_data[m_pos];
		m_pos += sizeof( T );
		return result;
	}

	fixed_constants_data_buffer* next;

private:
	u8 m_data[buffer_size];
	u32 m_pos;
};

struct effect_constant_storage : public quasi_singleton<effect_constant_storage> {
	typedef vector<data_indexer> indexers_vector_type;

	effect_constant_storage( );
	~effect_constant_storage( );
	void clear( );

	template < typename T >
	T* store_constant( T const value )
	{
		COMPILE_ASSERT( constant_type_traits<T>::size % 4 == 0, The_type_must_be_multiple_to_4_bytes );

		u32 class_id = constant_type_traits<T>::class_id;
		data_indexer to_find( (u32*)&value, class_id );
		indexers_vector_type::iterator found = std::lower_bound(
			m_indexers.begin( ),
			m_indexers.end( ),
			to_find,
			constant_data_predicate
		);

		while ( found != m_indexers.end( ) ) {
			if ( is_equal( found->data_ptr, (u32*)&value, sizeof( value ) / 4 ) ) {
				return (T*)found->data_ptr;
			}

			++found;
		}

		if ( !m_constant_buffer )
			m_constant_buffer = NEW( fixed_constants_data_buffer );
		if ( m_constant_buffer->is_overflowing( sizeof( T ) ) ) {
			fixed_constants_data_buffer* new_buffer = NEW( fixed_constants_data_buffer );
			new_buffer->next = m_constant_buffer;
			m_constant_buffer = new_buffer;
		}

		T* new_data = new( m_constant_buffer->place<T>( ) )( T );
		*new_data = value;

		data_indexer to_insert( (u32*)new_data, class_id );
		indexers_vector_type::iterator where_insert = std::lower_bound(
			m_indexers.begin( ),
			m_indexers.end( ),
			to_insert,
			constant_data_predicate
		);

		if ( where_insert != m_indexers.end( ) ) {
			m_indexers.insert( where_insert, 1, to_insert );
		}
		else
			m_indexers.push_back( to_insert );

		return new_data;
	}

private:
	bool is_equal( u32 const* left, u32 const* right, u32 const count );

	indexers_vector_type m_indexers;
	fixed_constants_data_buffer* m_constant_buffer;
};

template <>
inline bool* effect_constant_storage::store_constant<bool>( bool const value )
{
	return reinterpret_cast<bool*>( store_constant<u32>( u32( value ) ) );
}

STATIC_SIZE_ASSERT( data_indexer, 0x8 );
STATIC_SIZE_ASSERT( fixed_constants_data_buffer, 0x408 );
STATIC_SIZE_ASSERT( effect_constant_storage, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_CONSTANT_STORAGE_H_INCLUDED
