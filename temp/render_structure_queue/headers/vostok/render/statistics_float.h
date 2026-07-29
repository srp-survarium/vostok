////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATISTICS_FLOAT_H_INCLUDED
#define RENDER_STATISTICS_FLOAT_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_value<double>;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::render::statistics_group;

namespace vostok {
namespace render {

struct statistics_float : public statistics_value< double > {
	inline			statistics_float	( statistics_group* arg_0, pcstr arg_1 ) { /* no source */ }

	virtual	void	print				( fs_new::virtual_path_string& out_result ) override;

	inline			~statistics_float	( ) { /* no source */ }
}; // struct statistics_float

STATIC_SIZE_ASSERT(statistics_float, 0xD8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_FLOAT_H_INCLUDED
