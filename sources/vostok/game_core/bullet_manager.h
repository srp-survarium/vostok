////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BULLET_MANAGER_H_INCLUDED
#define BULLET_MANAGER_H_INCLUDED

#include <vostok/intrusive_mpmc_stack.h>
#include <vostok/unmanaged_allocation_resource.h>
#include <vostok/memory_single_size_buffer_allocator.h>

#include <vostok/game_core/bullet.h>
#include <vostok/game_core/weapon_ammunition.h>

namespace vostok {
namespace physics {
	struct world;
}
namespace tasks {
	class task_type;
}
}

namespace survarium {

class weapon_core;

class bullet_manager_engine;
class game_material_manager;
class hit_initiator;
class hit_receiver;


class bullet_manager {
public:
	explicit								bullet_manager				( game_material_manager* material_manager, physics::world* physics_world, bullet_manager_engine* engine );
											~bullet_manager				( );

			void							initialize					( );
			void							register_console_commands	( );

			void							tick						( u32 current_time_in_ms );

			void							fire						(
												float3 const&						position,
												float3 const&						velocity,
												weapon_ammunition_ptr const&		wa,
												weapon_core const&					wc,
												u32									current_time_in_ms,
												hit_initiator const*				initiator,
												hit_receiver const*					ignorable_object,
												bool								tracer
											);

	inline	bool							has_engine					( ) const { /* no source */ }

			void							add_decal					(
												resources::unmanaged_resource_ptr const&	decal,
												float										size,
												float3 const&								position,
												float3 const&								direction,
												float3 const&								normal,
												bool										is_front_face
											);

	inline	void							play_sound					( resources::unmanaged_resource_ptr const& arg_0, float3 const& arg_1 ) { /* no source */ }

			void							play_particle				(
												resources::unmanaged_resource_ptr const&	sound,
												float3 const&								position,
												float3 const&								direction,
												float3 const&								normal
											);

			void							update_tracer				(
												bullet*				bullet,
												float3 const&		position,
												float3 const&		direction,
												float				length
											);

			void							free_bullet					( bullet* bullet );

			bool							is_inside_collision_db		( float3 const& position ) const;

	inline	void							clear_decals				( ) { /* no source */ }

	inline	game_material_manager const&	get_material_manager		( ) const { /* no source */ }
	inline	physics::world&					get_physics_world			( ) const { /* no source */ }
	inline	float3 const&					get_gravity					( ) const { /* no source */ }
	inline	float							get_bullet_time_factor		( ) const { /* no source */ }

public:
	struct bullet_functor {
		inline	explicit	bullet_functor	( ) { /* no source */ }
		inline				~bullet_functor	( ) { /* no source */ }

	public:
		/* 0x0000 */	boost::function< void() >			functor;
		/* 0x0020 */	float3								position;
		/* 0x002c */	float3								direction;
		/* 0x0038 */	float3								normal;
		/* 0x0044 */	float								size;
		/* 0x0048 */	bullet_manager::bullet_functor*		next;
		/* 0x004c */	resources::unmanaged_resource_ptr	resource;
		/* 0x0050 */	bool								is_front_face;
	}; // struct bullet_functor

			void							add_decal_impl				(
												resources::unmanaged_resource_ptr const&	decal,
												float										size,
												float3 const&								position,
												float3 const&								direction,
												float3 const&								normal,
												bool										is_front_face
											);
			void							add_decal_impl				( bullet_manager::bullet_functor* functor );

public:
			void							play_sound_impl				( resources::unmanaged_resource_ptr const& sound, float3 const& position );
			void							play_particle_impl			(
												resources::unmanaged_resource_ptr const&	particle,
												float3 const&								position,
												float3 const&								direction,
												float3 const&								normal
											);

	inline	bool							attach_tracer_impl			( bullet* arg_0 ) { /* no source */ }
	inline	bool							detach_tracer_impl			( bullet* arg_0 ) { /* no source */ }
			void							update_tracer_impl			(
												u16					tracer_idx,
												float3 const&		position,
												float3 const&		direction,
												float				length
											);

			void							tick_bullets				( u32 start_index, u32 end_index, u32 current_time_in_ms );
			void							destroy_all_bullets			( pcstr args );

			void							set_max_bullets				( pcstr args );

			void							allocate_bullets_memory		( u32 new_max_bullets_count );
			void							bullets_memory_allocated	( resources::queries_result& queries );

			void							emit_bullet					(
												float3 const&						position,
												float3 const&						velocity,
												float								air_resistance,
												weapon_ammunition_ptr const&		wa,
												weapon_core const&					wc,
												u32									current_time_in_ms,
												hit_initiator const*				initiator,
												hit_receiver const*					ignorable_object,
												bool								tracer
											);

			void							destroy_bullet				( bullet**& destroying_bullet_iterator );
	inline	void							destroy_redundant_bullets	( ) { /* no source */ }
			void							destroy_one_bullet			( );

private:
	typedef intrusive_mpmc_stack< bullet_manager::bullet_functor, bullet_manager::bullet_functor, &bullet_manager::bullet_functor::next > bullet_functor_mpmc_stack;
	typedef memory::single_size_buffer_allocator< sizeof( bullet ), threading::simple_lock > bullets_allocator;

private:
	/* 0x0000 */	buffer_vector<bullet *>				m_bullets;
	/* 0x0008 */	float3								m_gravity;

private:
	class bullet_functor_mt_allocator : public boost::noncopyable {
	public:
		explicit				bullet_functor_mt_allocator	( void* buffer, u32 buffer_size )
		{
			// LOCALS
			// bullet_manager::bullet_functor* e<1>
			// bullet_manager::bullet_functor* i<1>
			// ******

			// FUNCTION BODY
			// <0xbe481>|0x031|+0x00c:'161'
			// <0xbe48d>|0x03d|+0x02e|[1]:'162'
			// <0xbe4bb>|0x06b|+0x062:'163'
			// ******
		}

		inline	void*			buffer						( ) const { /* no source */ }

		inline	bullet_functor*	allocate					( ) { /* no source */ }
				void			deallocate					( bullet_functor*& functor )
		{
			// FUNCTION BODY
			// <0xbe53a>|0x00a|+0x068:'180'
			// <0xbe5a2>|0x072|+0x009:'181'
			// ******
		}

		inline	void			swap						( bullet_functor_mt_allocator& arg_0 ) { /* no source */ }

				void*			malloc_impl					( u32 size )
		{
			return NULL;

			// FUNCTION BODY
			// <0>
			// <1>
			// <0xbe5c9>|0x009|+0x023:'194'
			// <0xbe5ec>|0x02c|+0x01a:'195'
			// ******
		}

		inline	void			free_impl					( void* arg_0 ) { /* no source */ }

	private:
		/* 0x0000 */	/* boost::noncopyable */
		/* 0x0000 */	bullet_functor_mpmc_stack	m_bullet_functors;
		/* 0x0008 */	void*						m_buffer;
	}; // class bullet_functor_mt_allocator

	/* 0x0018 */	bullet_manager::bullet_functor_mt_allocator		m_mt_stack_allocator;

private:
	/* 0x0028 */	bullet_functor_mpmc_stack						m_functors;
	/* 0x0030 */	resources::unmanaged_allocation_resource_ptr	m_bullets_memory_ptr;
	/* 0x0038 */	uninitialized_reference< bullets_allocator >	m_bullets_allocator_ref;

private:
	/* 0x0058 */	bullet_manager_engine*				m_engine;
	/* 0x005c */	game_material_manager*				m_game_material_manager;
	/* 0x0060 */	physics::world*						m_physics_world;
	/* 0x0064 */	tasks::task_type*					m_task_type;
	/* 0x0068 */	u32									m_max_bullets_count;
	/* 0x006c */	u32									m_max_bullets_decals_count;
	/* 0x0070 */	u32									m_current_decal_id;
	/* 0x0074 */	float								m_bullet_time_factor;
	/* 0x0078 */	float								m_air_resistance_epsilon;
}; // class bullet_manager

STATIC_SIZE_ASSERT(bullet_manager, 0x80);

} // namespace survarium

#endif // #ifndef BULLET_MANAGER_H_INCLUDED
