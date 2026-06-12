////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_OBJECT__H_INCLUDED
#define GAME_OBJECT__H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
class survarium::base_game_scene;

/* FORWARD REFS */
class boost::function<void __cdecl(survarium::game_object_ &)>;
class vostok::configs::binary_config_value;

namespace survarium {

class game_object_ : public resources::unmanaged_resource , public boost::noncopyable {
public:
			explicit			game_object_	( base_game_scene& s );

	virtual	void				load			(
									configs::binary_config_value const&		arg_0,
									pcstr									arg_1,
									boost::function< void( game_object_& ) >&	arg_2
								) = 0;

	virtual	void				insert			( ) { /* no source */ }
	virtual	void				remove			( ) { /* no source */ }

	inline	base_game_scene&	get_game_scene	( ) { /* no source */ }

	virtual						~game_object_	( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	base_game_scene&	m_game_scene;
}; // class game_object_

STATIC_SIZE_ASSERT(game_object_, 0x110);

} // namespace survarium

#endif // #ifndef GAME_OBJECT__H_INCLUDED
