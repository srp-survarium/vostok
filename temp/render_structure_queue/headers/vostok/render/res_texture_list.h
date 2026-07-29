////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_TEXTURE_LIST_H_INCLUDED
#define RENDER_RES_TEXTURE_LIST_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
typedef vostok::render::vector<vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >
	vostok::render::vector<vostok::render::res_texture_ptr >;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::fixed_vector<vostok::render::texture_slot,128>;

namespace vostok {
namespace render {

class res_texture_list : public resource_intrusive_base {
public:
	inline								res_texture_list	( ) { /* no source */ }
			explicit					res_texture_list	( fixed_vector< texture_slot, 128 > const& slots );
	inline								~res_texture_list	( ) { /* no source */ }

			void						destroy_impl		( ) const;

	inline	bool						equal				( res_texture_list const& arg_0 ) const { /* no source */ }
	inline	bool						equal				( fixed_vector< texture_slot, 128 > const& arg_0 ) const { /* no source */ }

			s32							compare				( res_texture_list const& base ) const;
			s32							compare				( fixed_vector< texture_slot, 128 > const& base ) const;

	inline	u32							size				( ) const { /* no source */ }

	inline	res_texture_ptr const&		operator[]			( const u32 arg_0 ) const { /* no source */ }
	inline	res_texture_ptr&			operator[]			( const u32 arg_0 ) { /* no source */ }

	inline	void						resize				( const u32 arg_0, res_texture_ptr const& arg_1 ) { /* no source */ }

	inline	res_texture_ptr const*		begin				( ) const { /* no source */ }
	inline	res_texture_ptr*			begin				( ) { /* no source */ }

	inline	res_texture_ptr const*		end					( ) const { /* no source */ }
	inline	res_texture_ptr*			end					( ) { /* no source */ }

	inline	bool						is_registered		( ) const { /* no source */ }
	inline	void						mark_registered		( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	vector< res_texture_ptr >		m_container;
	/* 0x0010 */	bool							m_is_registered;
}; // class res_texture_list

STATIC_SIZE_ASSERT(res_texture_list, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_TEXTURE_LIST_H_INCLUDED
