////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATISTICS_VALUE_H_INCLUDED
#define RENDER_STATISTICS_VALUE_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_base;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::render::statistics_group;

namespace vostok {
namespace render {

struct statistics_value< double > : public statistics_base {
	inline				statistics_value<double>	( statistics_group* group, pcstr name ) { /* no source */ }

	virtual	void		start						( ) override { /* no source */ }

	virtual	void		print						( fs_new::virtual_path_string& arg_0 ) override = 0;

	virtual	void		clear						( ) override { /* no source */ }

	inline	double		average						( ) { /* no source */ }

	inline				~statistics_value<double>	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_base */
	/* 0x0098 */	double		value;
	/* 0x00a0 */	double		min_value;
	/* 0x00a8 */	double		max_value;
	/* 0x00b0 */	u8			value_num_max_digits;
	/* 0x00b1 */	u8			min_value_num_max_digits;
	/* 0x00b2 */	u8			max_value_num_max_digits;
	/* 0x00b8 */	double		min_value_temp;
	/* 0x00c0 */	double		max_value_temp;
protected:
	/* 0x00c8 */	double		history[1];
	/* 0x00d0 */	u32			history_index;
	/* 0x00d4 */	u32			min_max_frame_index;
}; // struct statistics_value< double >

STATIC_SIZE_ASSERT(statistics_value< double >, 0xD8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_VALUE_H_INCLUDED
