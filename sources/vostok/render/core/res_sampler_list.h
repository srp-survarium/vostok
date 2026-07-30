#ifndef VOSTOK_RENDER_CORE_RES_SAMPLER_LIST_H_INCLUDED
#define VOSTOK_RENDER_CORE_RES_SAMPLER_LIST_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/fixed_vector.h>
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/sampler_slot.h>

namespace vostok {
namespace render {

class res_sampler_list : public resource_intrusive_base {
public:
	typedef vector<ID3D11SamplerState*> samplers_type;
	typedef samplers_type::const_iterator const_iterator;
	typedef samplers_type::iterator iterator;

	explicit res_sampler_list( fixed_vector<sampler_slot, 16> const& slots );
	~res_sampler_list( ) { }

	void destroy_impl( ) const;

	bool equal( res_sampler_list const& ) const
	{
		// STATE[STUB]
		return false;
	}

	bool equal( fixed_vector<sampler_slot, 16> const& ) const
	{
		// STATE[STUB]
		return false;
	}

	s32 compare( res_sampler_list const& base ) const;
	s32 compare( fixed_vector<sampler_slot, 16> const& base ) const;

	u32 size( ) const { return m_samplers.size( ); }
	ID3D11SamplerState* const& operator[]( u32 index ) const { return m_samplers[index]; }
	ID3D11SamplerState*& operator[]( u32 index ) { return m_samplers[index]; }
	const_iterator begin( ) const { return m_samplers.begin( ); }
	iterator begin( ) { return m_samplers.begin( ); }
	const_iterator end( ) const { return m_samplers.end( ); }
	iterator end( ) { return m_samplers.end( ); }

	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( )
	{
		R_ASSERT( !m_is_registered );
		m_is_registered = true;
	}

	void rebind( );

private:
	samplers_type m_samplers;
	vector<fixed_string<32> > m_names;
	bool m_is_registered;
};

typedef intrusive_ptr<
	res_sampler_list,
	resource_intrusive_base,
	threading::single_threading_policy
> res_sampler_list_ptr;

typedef intrusive_ptr<
	res_sampler_list const,
	resource_intrusive_base const,
	threading::single_threading_policy
> res_sampler_list_const_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_RES_SAMPLER_LIST_H_INCLUDED
