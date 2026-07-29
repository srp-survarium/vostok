////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_SAMPLER_LIST_H_INCLUDED
#define RENDER_RES_SAMPLER_LIST_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
class vostok::render::vector<ID3D11SamplerState *>;
class vostok::render::vector<vostok::fixed_string<32> >;

/* FORWARD REFS */
class vostok::fixed_vector<vostok::render::sampler_slot,16>;

namespace vostok {
namespace render {

class res_sampler_list : public resource_intrusive_base {
public:
			explicit						res_sampler_list	( fixed_vector< sampler_slot, 16 > const& slots );
	inline									~res_sampler_list	( ) { /* no source */ }

			void							destroy_impl		( ) const;

	inline	bool							equal				( res_sampler_list const& arg_0 ) const { /* no source */ }
	inline	bool							equal				( fixed_vector< sampler_slot, 16 > const& arg_0 ) const { /* no source */ }

			s32								compare				( res_sampler_list const& base ) const;
			s32								compare				( fixed_vector< sampler_slot, 16 > const& base ) const;

	inline	u32								size				( ) const { /* no source */ }

	inline	ID3D11SamplerState* const&		operator[]			( const u32 arg_0 ) const { /* no source */ }
	inline	ID3D11SamplerState*&			operator[]			( const u32 arg_0 ) { /* no source */ }

	inline	ID3D11SamplerState* const*		begin				( ) const { /* no source */ }
	inline	ID3D11SamplerState**			begin				( ) { /* no source */ }

	inline	ID3D11SamplerState* const*		end					( ) const { /* no source */ }
	inline	ID3D11SamplerState**			end					( ) { /* no source */ }

	inline	bool							is_registered		( ) const { /* no source */ }
	inline	void							mark_registered		( ) { /* no source */ }

			void							rebind				( );

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	vector< ID3D11SamplerState* >		m_samplers;
	/* 0x0010 */	vector< fixed_string< 32 > >		m_names;
	/* 0x001c */	bool								m_is_registered;
}; // class res_sampler_list

STATIC_SIZE_ASSERT(res_sampler_list, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_SAMPLER_LIST_H_INCLUDED
