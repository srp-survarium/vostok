// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BULLET_MANAGER_H_INCLUDED
#define BULLET_MANAGER_H_INCLUDED

#include <vostok/intrusive_mpmc_stack.h>
#include <vostok/unmanaged_allocation_resource.h>
#include <vostok/memory_single_size_buffer_allocator.h>

#include <vostok/game_core/bullet.h>
#include <vostok/game_core/bullet_manager_engine.h>
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

extern float s_bm_bullet_time_factor;

class weapon_core;

class bullet_manager_engine;
class game_material_manager;
struct hit_initiator;
struct hit_receiver;


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
												hit_initiator const* const			initiator,
												hit_receiver const* const			ignorable_object,
												bool								tracer
											);

	inline	bool							has_engine					( ) const { return m_engine != NULL; }

			void							add_decal					(
												resources::unmanaged_resource_ptr const&	decal,
												float										size,
												float3 const&								position,
												float3 const&								direction,
												float3 const&								normal,
												bool										is_front_face
											);

			void							play_sound					(
												resources::unmanaged_resource_ptr const&	sound,
												float3 const&								position
											);

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
												float const			length
											);

			void							free_bullet					( bullet* bullet );

			bool							is_inside_collision_db		( float3 const& position ) const;

	inline	void							clear_decals				( )
	{
		// sushi@TODO: Verify the full-ring removal model, engine guard and whether clearing resets the next decal ID.
		if ( !m_engine )
			return;
		for ( u32 i = 0; i < m_max_bullets_decals_count; ++i )
			m_engine->remove_decal( i );
	}

	inline	game_material_manager const&	get_material_manager		( ) const { return *m_game_material_manager; }
	inline	physics::world&					get_physics_world			( ) const { return *m_physics_world; }
	inline	float3 const&					get_gravity					( ) const { return m_gravity; }
	inline	float							get_bullet_time_factor		( ) const { return s_bm_bullet_time_factor; }

private:
	typedef buffer_vector<bullet*> bullets_type;

	struct bullet_functor {
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

			void							add_decal_impl				( bullet_manager::bullet_functor* const functor );
			void							add_decal_impl				(
												resources::unmanaged_resource_ptr const&	decal,
												float										size,
												float3 const&								position,
												float3 const&								direction,
												float3 const&								normal,
												bool										is_front_face
											);
			void							play_sound_impl				( resources::unmanaged_resource_ptr const& sound, float3 const& position );
			void							play_particle_impl			(
												resources::unmanaged_resource_ptr const&	particle,
												float3 const&								position,
												float3 const&								direction,
												float3 const&								normal
											);
	// sushi@TODO: Verify the original tracer-forwarder boundaries; retained emit/free callers establish the dispatch and own its guards.
	inline	bool							attach_tracer_impl			( bullet* bullet ) { return m_engine->attach_tracer( bullet ); }
	inline	bool							detach_tracer_impl			( bullet* bullet ) { return m_engine->detach_tracer( bullet ); }
			void							update_tracer_impl			(
												u16	const			tracer_idx,
												float3 const&		position,
												float3 const&		direction,
												float const			length
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
												hit_initiator const* const			initiator,
												hit_receiver const*	const			ignorable_object,
												bool								tracer
											);

			void							destroy_bullet				( buffer_vector<bullet*>::iterator const& destroying_bullet_iterator );
	inline	void							destroy_redundant_bullets	( );
			void							destroy_one_bullet			( );

private:
	typedef memory::single_size_buffer_allocator<
		sizeof( bullet ),
		threading::simple_lock >							bullets_allocator;

	typedef resources::unmanaged_allocation_resource_ptr	unmanaged_allocation_resource_ptr;
	typedef resources::unmanaged_resource_ptr				unmanaged_resource_ptr;

	typedef intrusive_mpmc_stack<
		bullet_manager::bullet_functor,
		bullet_manager::bullet_functor,
		&bullet_manager::bullet_functor::next >				bullet_functors_type;

private:
	// sushi@TODO: Resolve public boost base in the carcass versus private source; frozen target/base records compare identical.
	class bullet_functor_mt_allocator : private boost::noncopyable {
	public:
		typedef bullet_manager::bullet_functor bullet_functor;

		explicit				bullet_functor_mt_allocator	( void* const buffer, const u32 buffer_size ) : m_buffer( buffer )
		{
			ASSERT( UNKNOWN_EXPRESSION );
			for ( bullet_functor *i = static_cast<bullet_functor*>( buffer ), *e = i + buffer_size / sizeof( bullet_functor ) ; i != e ; ++i )
				m_bullet_functors.push( i );
		}
		// sushi@TODO: Find the original buffer-getter consumer; retain the existing non-owning pointer contract.
		inline	void*			buffer						( ) const { return m_buffer; }

		inline	bullet_functor*	allocate					( ) {
			// sushi@TODO: Verify the original allocation assertion and inline boundary; malloc_impl retains this post-pop assertion use.
			bullet_functor* result = m_bullet_functors.try_pop( );
			ASSERT( UNKNOWN_EXPRESSION_T( result ) );
			return result;
		}

				void			deallocate					( bullet_functor*& functor )
		{
			m_bullet_functors.push( functor );
			functor = NULL;
		}

		inline	void			swap						( bullet_functor_mt_allocator& other )
		{
			std::swap( m_bullet_functors, other.m_bullet_functors );
			std::swap( m_buffer, other.m_buffer );
		}

				void*			malloc_impl					( u32 size )
		{
			ASSERT_CMP_U			( size, <=, sizeof( bullet_functor ) );
			return					allocate( );
		}

		inline	void			free_impl					( void* pointer )
		{
			bullet_functor* functor = static_cast<bullet_functor*>( pointer );
			deallocate( functor );
		}

	private:
		/* 0x0000 */	/* boost::noncopyable */
		/* 0x0000 */	bullet_functors_type	m_bullet_functors;
		/* 0x0008 */	void*						m_buffer;
	}; // class bullet_functor_mt_allocator

private:
	/* 0x0000 */	buffer_vector<bullet *>							m_bullets;
	/* 0x0008 */	float3											m_gravity;
	/* 0x0018 */	bullet_manager::bullet_functor_mt_allocator		m_mt_stack_allocator;
	/* 0x0028 */	bullet_functors_type							m_functors;
	/* 0x0030 */	resources::unmanaged_allocation_resource_ptr	m_bullets_memory_ptr;
	/* 0x0038 */	uninitialized_reference< bullets_allocator >	m_bullets_allocator_ref;
	/* 0x0058 */	bullet_manager_engine*							m_engine;
	/* 0x005c */	game_material_manager*							m_game_material_manager;
	/* 0x0060 */	physics::world*									m_physics_world;
	/* 0x0064 */	tasks::task_type*								m_task_type;
	/* 0x0068 */	u32												m_max_bullets_count;
	/* 0x006c */	u32												m_max_bullets_decals_count;
	/* 0x0070 */	u32												m_current_decal_id;
	/* 0x0074 */	float											m_bullet_time_factor;
	/* 0x0078 */	float											m_air_resistance_epsilon;
}; // class bullet_manager

STATIC_SIZE_ASSERT(bullet_manager, 0x80);

} // namespace survarium

#endif // #ifndef BULLET_MANAGER_H_INCLUDED
