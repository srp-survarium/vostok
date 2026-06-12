////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef AI_SOUND_PLAYER_H_INCLUDED
#define AI_SOUND_PLAYER_H_INCLUDED

/* INCLUDES */
class vostok::sound::sound_producer;
class vostok::sound::sound_receiver;
class vostok::sound::world_user;
struct vostok::ai::sound_player;
enum vostok::sound::command_result_enum;
class vostok::ai::sound_collection_types;
class vostok::sound::sound_producer const* const;
class vostok::sound::sound_receiver const* const;
typedef vostok::intrusive_ptr<vostok::sound::sound_instance_proxy,vostok::sound::sound_instance_proxy,vostok::threading::single_threading_policy>
	vostok::sound::sound_instance_proxy_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;
class vostok::sound::sound_instance_proxy;

/* FORWARD REFS */
class vostok::memory::reader;
class vostok::memory::writer;
class survarium::ai_sound_player::sounds_collection_type;

namespace survarium {

class ai_sound_player : public ai::sound_player , public boost::noncopyable {
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

			ai_sound_player::sounds_collection_type const*	find						( ai::sound_collection_types sound_type ) const;

			sound::command_result_enum		on_finish_playing			( );

	inline	ai_sound_player::sounds_collection_type const*	sounds						( ) const { /* no source */ }

	inline	void							serialize					( ) { /* no source */ }
	inline	void							deserialize					( ) { /* no source */ }

private:
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

} // namespace survarium

#endif // #ifndef AI_SOUND_PLAYER_H_INCLUDED
