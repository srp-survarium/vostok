////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATISTICS_BASE_H_INCLUDED
#define RENDER_STATISTICS_BASE_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<128>;
struct vostok::render::statistics_base;
struct vostok::render::statistics_group;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::ui::font;

namespace vostok {
namespace render {

struct statistics_base : public boost::noncopyable {
					statistics_base			( statistics_group* group, pcstr name );
					~statistics_base		( );

	virtual	void	print					( fs_new::virtual_path_string& arg_0 ) = 0;

	virtual	void	start					( ) = 0;

	virtual	void	clear					( ) { /* no source */ }

	inline	u32		calc_max_string_width	( ui::font const* arg_0 ) { /* no source */ }

	/* 0x0004 */	/* boost::noncopyable */
protected:
	/* 0x0004 */	fixed_string< 128 >		m_name;
	/* 0x0090 */	statistics_base*		m_next;
	/* 0x0094 */	statistics_group*		m_group;
}; // struct statistics_base

STATIC_SIZE_ASSERT(statistics_base, 0x98);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_BASE_H_INCLUDED
