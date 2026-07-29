////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_COMPARE_MEMBER_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_COMPARE_MEMBER_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_texture_list const* const;

/* FORWARD REFS */
class vostok::fixed_vector<vostok::render::texture_slot,128>;

namespace vostok {
namespace render {

struct resource_manager::compare_member_predicate< res_texture_list > {
	inline	bool	operator()	( res_texture_list const* const arg_0, res_texture_list const* const arg_1 ) const { /* no source */ }
	inline	bool	operator()	(
						fixed_vector< texture_slot, 128 > const&	arg_0,
						res_texture_list const* const		arg_1
					) const { /* no source */ }
	inline	bool	operator()	(
						res_texture_list const* const		arg_0,
						fixed_vector< texture_slot, 128 > const&	arg_1
					) const { /* no source */ }
}; // struct resource_manager::compare_member_predicate< res_texture_list >

STATIC_SIZE_ASSERT(resource_manager::compare_member_predicate< res_texture_list >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_COMPARE_MEMBER_PREDICATE_H_INCLUDED
