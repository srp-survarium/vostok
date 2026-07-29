////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATISTICS_GROUP_H_INCLUDED
#define RENDER_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<128>;
struct vostok::render::statistics_base;
struct vostok::render::statistics_group;

/* FORWARD REFS */
class vostok::ui::font;
class vostok::ui::world;

namespace vostok {
namespace render {

struct statistics_group : public boost::noncopyable {
			explicit	statistics_group	( pcstr group_name );
						~statistics_group	( );

	inline	u32			render				( ui::world& arg_0, u32 arg_1, u32 arg_2 ) { /* no source */ }

			void		start				( );

	inline	void		clear				( ) { /* no source */ }

	inline	void		calc_sizes			( ui::font const* arg_0, u32& arg_1, u32& arg_2 ) { /* no source */ }

	inline	u32			get_num_text_lines	( ) const { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	statistics_base*		first_statistics;
private:
	/* 0x0004 */	fixed_string< 128 >		m_name;
	/* 0x0090 */	statistics_group*		m_next;
}; // struct statistics_group

STATIC_SIZE_ASSERT(statistics_group, 0x94);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_GROUP_H_INCLUDED
