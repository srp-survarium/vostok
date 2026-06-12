////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_OBJECT_STATIC_H_INCLUDED
#define GAME_OBJECT_STATIC_H_INCLUDED

/* INCLUDES */
class survarium::game_object_;

/* FORWARD REFS */
class boost::function<void __cdecl(survarium::game_object_ &)>;
class vostok::configs::binary_config_value;
class survarium::base_game_scene;

namespace survarium {

class game_object_static : public game_object_ {
public:
			explicit	game_object_static	( base_game_scene& s );

	virtual	void		load				(
							configs::binary_config_value const&		t,
							pcstr									project_resources_path,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual				~game_object_static	( ) { /* no source */ }

private:
	/* 0x0000 */	/* game_object_ */
	/* 0x0110 */	float4x4	m_transform;
}; // class game_object_static

STATIC_SIZE_ASSERT(game_object_static, 0x150);

} // namespace survarium

#endif // #ifndef GAME_OBJECT_STATIC_H_INCLUDED
