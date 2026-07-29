////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_COLLISION_RESULT_USER_DATA_EQUALLS_TO_H_INCLUDED
#define RENDER_CULLING_COLLISION_RESULT_USER_DATA_EQUALLS_TO_H_INCLUDED

/* INCLUDES */
struct stlp_std::unary_function<vostok::collision::triangle_result const &,bool>;

/* FORWARD REFS */
class vostok::collision::triangle_result;

namespace vostok {
namespace render {
namespace culling {

struct collision_result_user_data_equalls_to : public std::unary_function< collision::triangle_result const&, bool > {
	inline	explicit	collision_result_user_data_equalls_to( u32 arg_0 ) { /* no source */ }

	inline	bool		operator()	( collision::triangle_result const& arg_0 ) const { /* no source */ }

	inline				~collision_result_user_data_equalls_to( ) { /* no source */ }

	/* 0x0000 */	/* std::unary_function< collision::triangle_result const&, bool > */
private:
	/* 0x0000 */	u32		m_user_data;
}; // struct collision_result_user_data_equalls_to

STATIC_SIZE_ASSERT(collision_result_user_data_equalls_to, 0x4);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_COLLISION_RESULT_USER_DATA_EQUALLS_TO_H_INCLUDED
