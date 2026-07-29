////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATISTICS_INT_H_INCLUDED
#define RENDER_STATISTICS_INT_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_value<int>;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::render::statistics_group;

namespace vostok {
namespace render {

struct statistics_int : public statistics_value< int > {
	inline			statistics_int	( statistics_group* arg_0, pcstr arg_1 ) { /* no source */ }

	virtual	void	print			( fs_new::virtual_path_string& out_result ) override;

	inline			~statistics_int	( ) { /* no source */ }
}; // struct statistics_int

STATIC_SIZE_ASSERT(statistics_int, 0xBC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_INT_H_INCLUDED
