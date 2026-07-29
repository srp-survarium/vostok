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

struct statistics_value< int > : public statistics_base {
	inline			statistics_value<int>	( statistics_group* arg_0, pcstr arg_1 ) { /* no source */ }

	virtual	void	start					( ) override { /* no source */ }

	virtual	void	print					( fs_new::virtual_path_string& arg_0 ) override = 0;

	virtual	void	clear					( ) override { /* no source */ }

	inline	s32		average					( ) { /* no source */ }

	inline			~statistics_value<int>	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_base */
	/* 0x0098 */	s32		value;
	/* 0x009c */	s32		min_value;
	/* 0x00a0 */	s32		max_value;
	/* 0x00a4 */	u8		value_num_max_digits;
	/* 0x00a5 */	u8		min_value_num_max_digits;
	/* 0x00a6 */	u8		max_value_num_max_digits;
	/* 0x00a8 */	s32		min_value_temp;
	/* 0x00ac */	s32		max_value_temp;
protected:
	/* 0x00b0 */	s32		history[1];
	/* 0x00b4 */	u32		history_index;
	/* 0x00b8 */	u32		min_max_frame_index;
}; // struct statistics_value< int >

STATIC_SIZE_ASSERT(statistics_value< int >, 0xBC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_VALUE_H_INCLUDED
