////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_TAB_H_INCLUDED
#define OPTIONS_TAB_H_INCLUDED

/* INCLUDES */
class survarium::game;
class survarium::options_item_base;
enum survarium::options_enum;
typedef vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base>
	survarium::flash_movie_resource_ptr;
class survarium::flash_movie_resource;

namespace survarium {

enum survarium::options_enum
{
	gameplay_options_type		= 0x0,
	controllers_options_type	= 0x1,
	video_options_type			= 0x2,
	sound_options_type			= 0x3,
	options_count				= 0x4,
};

class options_tab : public boost::noncopyable {
public:
											options_tab		( game& g, flash_movie_resource_ptr& movie, options_enum type );
											~options_tab	( );

			void							apply			( flash_movie_resource_ptr& movie );

			void							revert			( flash_movie_resource_ptr& movie );

			void							initialize_data	( flash_movie_resource_ptr& movie );

	inline	options_item_base*				option_by_id	( u8 arg_0 ) { /* no source */ }

	inline	options_enum					type			( ) { /* no source */ }

	inline	game&							get_game		( ) { /* no source */ }
	inline	flash_movie_resource_ptr&		get_movie		( ) { /* no source */ }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	options_item_base**				m_options;
	/* 0x0004 */	u8								m_options_count;
	/* 0x0008 */	options_enum					m_type;
	/* 0x000c */	game&							m_game;
	/* 0x0010 */	flash_movie_resource_ptr&		m_movie;
}; // class options_tab

STATIC_SIZE_ASSERT(options_tab, 0x14);

} // namespace survarium

#endif // #ifndef OPTIONS_TAB_H_INCLUDED
