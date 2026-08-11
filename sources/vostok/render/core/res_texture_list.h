#ifndef VOSTOK_RENDER_CORE_RES_TEXTURE_LIST_H_INCLUDED
#define VOSTOK_RENDER_CORE_RES_TEXTURE_LIST_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/texture_slot.h>

namespace vostok {
namespace render {

class texture_slot;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

class res_texture_list : public resource_intrusive_base {
public:
	typedef vector<res_texture_ptr> textures_type;
	typedef textures_type::const_iterator const_iterator;
	typedef textures_type::iterator iterator;

	res_texture_list( ) :
		m_is_registered( false )
	{
	}

	explicit res_texture_list( fixed_vector<texture_slot, 128> const& slots );

	~res_texture_list( ) { }

	void destroy_impl( ) const;

	bool equal( res_texture_list const& base ) const
	{
		if ( m_container.size( ) != base.m_container.size( ) )
			return false;

		for ( u32 cmp = 0, size = m_container.size( ); cmp < size; ++cmp ) {
			if ( m_container[cmp] != base.m_container[cmp] )
				return false;
		}

		return true;
	}

	bool equal( fixed_vector<texture_slot, 128> const& base ) const
	{
		u32 const base_size = base.size( );
		if ( base_size != m_container.size( ) )
			return false;

		for ( u32 i = 0; i < base_size; ++i )
			if ( m_container[i] != base[i].texture )
				return false;

		return true;
	}

	s32 compare( res_texture_list const& base ) const;
	s32 compare( fixed_vector<texture_slot, 128> const& base ) const;

	u32 size( ) const { return m_container.size( ); }

	res_texture_ptr const& operator[]( u32 index ) const
	{
		return m_container[index];
	}

	res_texture_ptr& operator[]( u32 index )
	{
		return m_container[index];
	}

	void resize( u32 size, res_texture_ptr const& value )
	{
		m_container.resize( size, value );
	}

	const_iterator begin( ) const { return m_container.begin( ); }
	iterator begin( ) { return m_container.begin( ); }
	const_iterator end( ) const { return m_container.end( ); }
	iterator end( ) { return m_container.end( ); }

	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }

private:
	textures_type	m_container;
	bool			m_is_registered;
};

typedef intrusive_ptr<
	res_texture_list,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_list_ptr;

STATIC_SIZE_ASSERT( res_texture_list, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_RES_TEXTURE_LIST_H_INCLUDED
