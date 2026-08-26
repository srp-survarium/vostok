////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef AI_SOUND_PLAYER_H_INCLUDED
#define AI_SOUND_PLAYER_H_INCLUDED

#include <vostok/ai/sound_player.h>	// brings ai/sound_collection_types.h (the enum the dump inlines here)
#include <vostok/sound/sound.h>	// sound::command_result_enum
#include <vostok/sound/sound_emitter.h>	// sound_emitter_ptr / sound_instance_proxy_ptr
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace memory {
	class reader;
	class writer;
} // namespace memory
namespace sound {
	class sound_producer;
	class sound_receiver;
	class world_user;
} // namespace sound
} // namespace vostok

namespace survarium {

class ai_sound_player : public ai::sound_player , private boost::noncopyable {
public:
											ai_sound_player				(
												resources::unmanaged_resource_ptr&		scene,
												u32										sounds_count,
												sound::world_user&						user,
												sound::sound_producer const* const		producer,
												sound::sound_receiver const* const		ignorable_receiver
											);
	virtual									~ai_sound_player			( );

	virtual	void							play						(
												ai::sound_collection_types		sound_type,
												bool							sound_is_positioned,
												float3 const&					position
											) override;
	virtual	void							play						(
												resources::unmanaged_resource_ptr		sound_to_be_played,
												boost::function< void() > const&		finish_callback,
												float3 const&							position
											) override;

	virtual	void							play_once					(
												ai::sound_collection_types		sound_type,
												bool							sound_is_positioned,
												float3 const&					position
											) override;

	virtual	void							tick						( ) override;

	virtual	void							clear_resources				( ) override;

			void							on_active_sound_serialized	( memory::writer* sound_thread_writer, memory::writer* current_thread_writer );
			void							on_active_sound_deserialized( memory::reader* reader, void* buf );

public:
	struct sounds_collection_type {
					sounds_collection_type	(
						ai_sound_player*				parent,
						ai::sound_collection_types		collection_type,
						sound::sound_emitter_ptr		emitter_ptr,
						u32								collection_priority
					);

	public:
		/* 0x0000 */	ai::sound_collection_types		type;
		/* 0x0004 */	resources::child_resource_ptr< sound::sound_emitter, resources::unmanaged_intrusive_base >	emitter;
		/* 0x000c */	u32								priority;
	}; // struct sounds_collection_type

private:
			sounds_collection_type const*		find						( ai::sound_collection_types sound_type ) const;

			sound::command_result_enum		on_finish_playing			( );

	// the sounds array trails the object (m_sounds_count entries)
	inline	sounds_collection_type const*	sounds						( ) const { return reinterpret_cast< sounds_collection_type const* >( this + 1 ); }

	inline	void							serialize					( ) { /* no source */ }
	inline	void							deserialize					( ) { /* no source */ }

	/* 0x0000 */	/* ai::sound_player */
	/* 0x0190 */	/* boost::noncopyable */
	/* 0x0190 */	sound::sound_instance_proxy_ptr			m_active_sound;
	/* 0x0194 */	resources::unmanaged_resource_ptr&		m_scene;
	/* 0x0198 */	sound::world_user&						m_user;
	/* 0x019c */	sound::sound_producer const*			m_sound_producer;
	/* 0x01a0 */	sound::sound_receiver const* const		m_ignorable_receiver;
	/* 0x01a4 */	const u32								m_sounds_count;
}; // class ai_sound_player

STATIC_SIZE_ASSERT(ai_sound_player, 0x1A8);
STATIC_SIZE_ASSERT(ai_sound_player::sounds_collection_type, 0x10);

} // namespace survarium

#endif // #ifndef AI_SOUND_PLAYER_H_INCLUDED
