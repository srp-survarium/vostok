////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_PLAYER_COOK_H_INCLUDED
#define SOUND_PLAYER_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
struct vostok::sound::world;
enum vostok::resources::class_id_enum;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace survarium {

class sound_player_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					sound_player_cook	( sound::world* world, resources::class_id_enum class_id );

	virtual	void	translate_query		( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource		( resources::resource_base* resource ) override;

			void	on_sounds_loaded	( resources::queries_result& data, configs::binary_config_ptr config );
			void	on_config_loaded	( resources::queries_result& data );

	virtual			~sound_player_cook	( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	sound::world*		m_world;
}; // class sound_player_cook

STATIC_SIZE_ASSERT(sound_player_cook, 0x24);

} // namespace survarium

#endif // #ifndef SOUND_PLAYER_COOK_H_INCLUDED
