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
	typedef vector<data_indexer> indexers_type;

	effect_constant_storage( );
	~effect_constant_storage( );
	void clear( );

	template < typename T >
	T* store_constant( T const value )
	{
		u32 const class_id = constant_type_traits<T>::class_id;
		data_indexer sought( reinterpret_cast<u32*>( const_cast<T*>( &value ) ), class_id );
		indexers_type::iterator found = std::lower_bound(
			m_indexers.begin( ),
			m_indexers.end( ),
			sought,
			constant_data_predicate
		);

		for ( ; found != m_indexers.end( ); ++found )
			if ( is_equal( found->data_ptr, sought.data_ptr, sizeof( T ) / sizeof( u32 ) ) )
				return reinterpret_cast<T*>( found->data_ptr );

		if ( !m_constant_buffer )
			m_constant_buffer = NEW( fixed_constants_data_buffer );
		if ( m_constant_buffer->is_overflowing( sizeof( T ) ) ) {
			fixed_constants_data_buffer* buffer = NEW( fixed_constants_data_buffer );
			buffer->next = m_constant_buffer;
			m_constant_buffer = buffer;
		}

		T* stored = new( m_constant_buffer->place<T>( ) ) T( value );
		data_indexer inserted( reinterpret_cast<u32*>( stored ), class_id );
		indexers_type::iterator position = std::lower_bound(
			m_indexers.begin( ),
			m_indexers.end( ),
			inserted,
			constant_data_predicate
		);
		m_indexers.insert( position, inserted );
		return stored;
	}

private:
	bool is_equal( u32 const* left, u32 const* right, u32 count );

	indexers_type m_indexers;
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
