////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/bullet_manager.h>

#include <vostok/console_command.h>
#include <vostok/physics/world.h>
#include <vostok/tasks_system.h>
#include <vostok/game_core/bullet_manager_engine.h>

namespace survarium {

static float s_bm_current_air_resistance = 1.0f; // sushi@TODO: Move somewhere?
static float s_bm_bullet_time_factor	 = 1.0f; // sushi@TODO: Move somewhere?

// STATE[100%|DONE]
bullet_manager::bullet_manager(
	game_material_manager*	material_manager,
	physics::world*			physics_world,
	bullet_manager_engine*	engine
) :
	m_bullets					( NULL, 0 ),
	m_gravity					( 0, -9.81f, 0 ),
	m_mt_stack_allocator		( NULL, 0 ),
	m_engine					( engine ),
	m_game_material_manager		( material_manager ),
	m_physics_world				( physics_world ),
	m_task_type					( tasks::create_new_task_type( "bullet", tasks::task_type_flags_no_self_parallelization_hint ) ),
	m_max_bullets_count			( 0 ),
	m_max_bullets_decals_count	( 64 ),
	m_current_decal_id			( 0 ),
	m_bullet_time_factor		( 1.0f ),
	m_air_resistance_epsilon	( 0.1f )
#ifndef MASTER_GOLD // sushi@NOTE: Fields are not implemented in bullet_manager. Copy them from the OG implementation.
	,
	m_is_draw_debug				( true ),
	m_is_draw_trajectories		( true ),
	m_is_draw_collision_trajectories( false ),
	m_is_draw_decals_data		( true ),
	m_is_draw_collision_points	( true ),
	m_bullet_trajectories_points( vectora< render::vertex_colored >( g_allocator ) )

#endif // #ifndef MASTER_GOLD
{
	initialize( );
	register_console_commands( );
}

// STATE[100%|DONE]
bullet_manager::~bullet_manager( )
{
	if ( m_bullets_allocator_ref.initialized( ) )
		m_bullets_allocator_ref.destroy( ); // sushi@NOTE: Shouldn't this be in destructor?
}

// STATE[100%|DONE]
void bullet_manager::initialize( )
{
	allocate_bullets_memory( 192 ); // sushi@TODO: Constant for MAX_BULLETS_COUNT?
}

// STATE[100%|DONE]
void bullet_manager::register_console_commands( )
{
	static vostok::console_commands::cc_delegate	displace_all_bullets_cc	( "bullets_manager_displace_all_bullets",	boost::bind( &bullet_manager::destroy_all_bullets, this, _1 ),	true, console_commands::command_type_engine_internal );
	static vostok::console_commands::cc_delegate	set_max_bullets_cc		( "bullets_manager_set_max_bullets",		boost::bind( &bullet_manager::set_max_bullets, this, _1 ),		true, console_commands::command_type_engine_internal );
}


float bullet_manager::get_bullet_time_factor( ) const
{
	return s_bm_bullet_time_factor;
}

struct redundant_bullet_predicate {
	// STATE[INLINED]: ctor inlined into tick's remove_if; no standalone body.
	inline	explicit	redundant_bullet_predicate	( bullet_manager& bullet_manager ) : bullet_manager( &bullet_manager ) { }

	// STATE[94.85%|PARTIAL]: operator!= inline-vs-call (target folds to !(operator==)).
			bool		operator()					( bullet* bullet )
	{
		if ( bullet->get_start_velocity( ) != float3( 0, 0, 0 ) )
			return false;

		bullet_manager->free_bullet( bullet );
		return true;

		// STRUCTURE DIFF: target 4 / base 4 stmts
		// SIZE -0x2 | 96 | if ( bullet->get_start_velocity( ) != float3( 0, 0, 0 ) )
		// VERDICT: STRUCTURE MATCH (shape ok) - target has no standalone float3 operator!=, folds to !(operator==) + branch flip; LTCG inline-vs-call, non-steerable.
	}

public:
	/* 0x0000 */	survarium::bullet_manager*		bullet_manager;
}; // struct redundant_bullet_predicate

// STATE[87.8%|PARTIAL]: delete_helper wrapper inline-vs-call LTCG.
void bullet_manager::free_bullet( bullet* bullet )
{
	if ( m_engine && bullet->m_tracer_idx != u16(-1) )
		m_engine->detach_tracer( bullet );

	VOSTOK_DELETE_IMPL( *m_bullets_allocator_ref, bullet );

	// STRUCTURE DIFF: target 3 / base 3 stmts
	// SIZE +0x9 | 119 | VOSTOK_DELETE_IMPL( *m_bullets_allocator_ref, bullet );
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls delete_helper<> wrapper out-of-line, base inlines it to delete_helper_impl<> (+predicate bool); whole-program LTCG, non-steerable.
}

// STATE[99.86%|PARTIAL]: frame/slot LTCG residual only.
void bullet_manager::tick( u32 current_time_in_ms )
{
	const u32 bullets_count = m_bullets.size( );
	if ( bullets_count )
	{
		const u32 granularity	= 256;
		const u32 n				= bullets_count / granularity;
		if ( bullets_count >= granularity )
			for ( u32 i = 0; i < n; ++i )
				tasks::spawn_task	( boost::bind( &bullet_manager::tick_bullets, this, i*16, (i + 1)*16 , current_time_in_ms ), m_task_type );

		const u32 start_index = n * granularity;
		tick_bullets			( start_index, start_index + bullets_count % granularity, current_time_in_ms );

		tasks::wait_for_all_children	( );

		while ( !m_functors.empty( ) )
		{
			bullet_manager::bullet_functor* functor = m_functors.try_pop( );
			ASSERT( UNKNOWN_EXPRESSION( functor ) );
			functor->functor( );
			VOSTOK_DELETE_IMPL( m_mt_stack_allocator, functor );
		}

		m_bullets.erase			(
			std::remove_if(
				m_bullets.begin( ),
				m_bullets.end( ),
				redundant_bullet_predicate( *this )
			),
			m_bullets.end( )
		);
	}

	// STRUCTURE DIFF: target 17 / base 17 stmts (no diverging rows)
	// VERDICT: STRUCTURE MATCH - residual is frame/slot bytes only (LTCG stack lowering), non-steerable.
}

// STATE[100%|DONE]
void bullet_manager::fire(
	float3 const&					position,
	float3 const&					velocity,
	weapon_ammunition_ptr const&	wa,
	weapon_core const&				wc,
	const u32						current_time_in_ms,
	hit_initiator const* const		initiator,
	hit_receiver const* const		ignorable_object,
	bool							tracer
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	emit_bullet( position, velocity, s_bm_current_air_resistance, wa, wc, current_time_in_ms, initiator, ignorable_object, tracer );
}

// STATE[98.97%|PARTIAL]: bullet_functor_mt_allocator::malloc_impl inline-vs-call LTCG (4-byte frame).
void bullet_manager::add_decal(
	resources::unmanaged_resource_ptr const&	decal,
	float										size,
	float3 const&								position,
	float3 const&								direction,
	float3 const&								normal,
	bool										is_front_face
)
{
	if ( m_engine && decal.c_ptr( ) )
	{
		bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL( m_mt_stack_allocator, bullet_manager::bullet_functor );

		functor->resource		= decal;
		functor->position		= position;
		functor->direction		= direction;
		functor->normal			= normal;
		functor->size			= size;
		functor->is_front_face	= is_front_face;
		functor->functor		= boost::bind( &bullet_manager::add_decal_impl, this, functor );
		m_functors.push( functor );
	}

	// STRUCTURE DIFF: target 10 / base 10 stmts
	// SIZE -0x2 | 197 | bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL( m_mt_stack_allocator, bullet_manager::bullet_functor );
	// SIZE +0x6 | 206 | m_functors.push( functor );
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls malloc_impl out-of-line (push 58h), base inlines to try_pop; the push row is disp8/disp32 slot cascade of the 4-byte frame shift; LTCG, non-steerable.
}

// STATE[100%|DONE]
void bullet_manager::add_decal_impl( bullet_manager::bullet_functor* functor )
{
	add_decal_impl( functor->resource, functor->size, functor->position, functor->direction, functor->normal, functor->is_front_face );
}

// STATE[100%|DONE]
void bullet_manager::add_decal_impl(
	resources::unmanaged_resource_ptr const&	decal,
	float										size,
	float3 const&								position,
	float3 const&								direction,
	float3 const&								normal,
	bool										is_front_face
)
{
	const float depth = 0.1f;

	m_engine->add_decal(
		decal,
		m_current_decal_id++,
		size,
		depth,
		position,
		direction,
		normal,
		is_front_face
	);

	if ( m_current_decal_id == m_max_bullets_decals_count )
		m_current_decal_id = 0;
}

// STATE[MISSING|INLINED]
void bullet_manager::play_sound( resources::unmanaged_resource_ptr const& sound, float3 const& position )
{
	if ( m_engine )
	{
		bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL( m_mt_stack_allocator, bullet_manager::bullet_functor );

		functor->resource	= sound;
		functor->position	= position;

		functor->functor = boost::bind(
			&bullet_manager::play_sound_impl,
			this,
			functor->resource,
			functor->position
		);
		m_functors.push( functor );
	}
}

// STATE[100%|DONE]
void bullet_manager::play_sound_impl( resources::unmanaged_resource_ptr const& sound, float3 const& position )
{
	m_engine->play_sound( sound, position );
}

// STATE[98.54%|PARTIAL]: boost::cref bind args fixed; residual is malloc_impl inline-vs-call LTCG.
void bullet_manager::play_particle(
	resources::unmanaged_resource_ptr const&	sound,
	float3 const&								position,
	float3 const&								direction,
	float3 const&								normal
)
{
	if ( m_engine )
	{
		bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL( m_mt_stack_allocator, bullet_manager::bullet_functor );

		functor->resource	= sound;
		functor->position	= position;
		functor->direction	= direction;
		functor->direction.normalize( );
		functor->direction	= normal;
		functor->normal.normalize( );

		functor->functor = boost::bind(
			&bullet_manager::play_particle_impl,
			this,
			boost::cref( functor->resource ),
			boost::cref( functor->position ),
			boost::cref( functor->direction ),
			boost::cref( functor->normal )
		);
		m_functors.push( functor );
	}

	// STRUCTURE DIFF: target 10 / base 10 stmts
	// SIZE +0x2 | 287 | bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL( m_mt_stack_allocator, bullet_manager::bullet_functor );
	// VERDICT: STRUCTURE MATCH (shape ok) - malloc_impl inline-vs-call (base inlines to try_pop); LTCG, non-steerable.
}

// STATE[100%|DONE]
void bullet_manager::play_particle_impl(
	resources::unmanaged_resource_ptr const&	particle,
	float3 const&								position,
	float3 const&								direction,
	float3 const&								normal
)
{
	m_engine->play_particle( particle, position, direction, normal );
}

// STATE[99.01%|PARTIAL]: boost::cref bind args fixed; residual is malloc_impl inline-vs-call LTCG (4-byte frame).
void bullet_manager::update_tracer(
	bullet*				bullet,
	float3 const&		position,
	float3 const&		direction,
	const float			length
)
{
	if ( m_engine )
	{
		bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL( m_mt_stack_allocator, bullet_manager::bullet_functor );
		functor->position = position;
		functor->direction = direction;
		functor->functor = boost::bind(
			&bullet_manager::update_tracer_impl,
			this,
			bullet->m_tracer_idx,
			boost::cref( functor->position ),
			boost::cref( functor->direction ),
			length
		);
		m_functors.push( functor );
	}

	// STRUCTURE DIFF: target 6 / base 6 stmts
	// SIZE -0x2 | 338 | bullet_manager::bullet_functor* functor = VOSTOK_NEW_IMPL( m_mt_stack_allocator, bullet_manager::bullet_functor );
	// SIZE +0x6 | 348 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - malloc_impl inline-vs-call (target push 58h + out-of-line call, base try_pop); the bind row is disp8/disp32 slot cascade of the frame shift; LTCG, non-steerable.
}

// STATE[100%|DONE]
void bullet_manager::update_tracer_impl(
	const u16			tracer_idx,
	float3 const&		position,
	float3 const&		direction,
	const float			length
)
{
	m_engine->update_tracer( tracer_idx, position, direction, length );
}

// STATE[100%|DONE]
void bullet_manager::tick_bullets( u32 start_index, u32 end_index, u32 current_time_in_ms )
{
	bullets_type::iterator			current = m_bullets.begin( ) + start_index;
	bullets_type::iterator const	end		= m_bullets.begin( ) + end_index;

	for ( ; current != end; ++current )
		(*current)->tick( current_time_in_ms );
}

// STATE[100%|DONE]
void bullet_manager::destroy_all_bullets( pcstr args )
{
	VOSTOK_UNREFERENCED_PARAMETER( args );
	while ( !m_bullets.empty( ) )	// sushi@NOTE: O(n^2)
		destroy_bullet( m_bullets.begin( ) );
}

// STATE[100%|DONE]
void bullet_manager::set_max_bullets( pcstr args )
{
	int new_bullets_count;
	if( VOSTOK_SSCANF( args, "%d", &new_bullets_count ) == EOF )
		return;

	allocate_bullets_memory			( new_bullets_count );
}

#define CALLBACKS_PER_BULLET 6

// STATE[93.48%|PARTIAL]: query_create_resources promoted-convention arg assignment LTCG.
// sushi@NOTE: I wonder why this waits only for when allocator is initialized
void bullet_manager::allocate_bullets_memory( u32 new_max_bullets_count )
{
	m_max_bullets_count				= new_max_bullets_count;

	resources::creation_request request	(
		"bullets_memory",
		m_max_bullets_count * ( sizeof( bullet ) + sizeof( bullet* ) + CALLBACKS_PER_BULLET * sizeof( bullet_functor ) ),
		resources::unmanaged_allocation_class
	);

	if( m_bullets_allocator_ref.initialized( ) )
	{
		resources::query_create_resources			(
			&request,
			1,
			boost::bind( &bullet_manager::bullets_memory_allocated, this, _1 ),
			g_allocator
		);
	}
	else
	{
		resources::query_create_resources_and_wait	(
			&request,
			1,
			boost::bind( &bullet_manager::bullets_memory_allocated, this, _1 ),
			g_allocator
		);
	}

	// STRUCTURE DIFF: target 6 / base 6 stmts
	// SIZE -0x2 | 419 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - LTCG promoted query_create_resources picked different register params per build (target ecx=count/eax=&request + push 0/0, base xor eax/xor ecx + count/&request on stack); call-boundary convention, non-steerable.
}

// STATE[95.32%|PARTIAL]: bool-const &&-temp, CONSTRUCT_REFERENCE arg and swap order fixed (90.14 -> 95.32); see embed.
// sushi@NOTE:
//	* static_cast_resource_ptr for casting unmanaged resources. Also see what IDA generated to match similar cases in the future.
void bullet_manager::bullets_memory_allocated( resources::queries_result& queries )
{   // sushi@TODO: Note that not all memory is currently being used. Specifically, yes: particle, decal, update_tracer ; me: sound ; no: attach_tracer, detach_tracer

	R_ASSERT								( queries[0].is_successful( ) );
	unmanaged_allocation_resource_ptr new_bullets_memory_ptr = static_cast_resource_ptr<unmanaged_allocation_resource_ptr>( queries[0].get_unmanaged_resource() );
	{	// sushi@TODO: ???
		pbyte pointer = new_bullets_memory_ptr->buffer;

		bool const is_realocation = m_bullets_allocator_ref.initialized( ) && m_bullets_allocator_ref->allocated_size( ) > 0;
		if ( is_realocation )
		{
			for ( u32 i = m_bullets_allocator_ref->allocated_size( ) / ( sizeof( bullet ) + sizeof( bullet* ) ); i > m_max_bullets_count; --i )
				destroy_one_bullet			( );

			bullets_allocator new_bullets_allocator		= bullets_allocator( pointer, m_max_bullets_count * sizeof( bullet ) );

			pointer							+= m_max_bullets_count * sizeof( bullet );	// sushi@NOTE: Memory for bullets and then the vector pointing to bullets
			bullets_type new_bullets_list	( pointer, m_max_bullets_count );

			while ( !m_bullets.empty( ) )
			{
				bullet*  old_bullet			= m_bullets[0];
				new_bullets_list.push_back	( VOSTOK_NEW_IMPL( new_bullets_allocator, bullet ) ( *old_bullet ) );
				VOSTOK_DELETE_IMPL			( *m_bullets_allocator_ref, old_bullet );
			}

			new_bullets_allocator.swap		( *m_bullets_allocator_ref );
			m_bullets.swap					( new_bullets_list );
		}
		else
		{
			if( m_bullets_allocator_ref.initialized( ) )
				VOSTOK_DESTROY_REFERENCE	( m_bullets_allocator_ref );

			VOSTOK_CONSTRUCT_REFERENCE		( m_bullets_allocator_ref, bullets_allocator )( pointer, m_max_bullets_count * sizeof( bullet ) );

			pointer							+= m_max_bullets_count * sizeof( bullet );
			bullets_type new_bullets_list	( pointer, m_max_bullets_count );
			m_bullets.swap					( new_bullets_list );
		}

		pointer								+= m_max_bullets_count * sizeof( bullet* );
		bullet_manager::bullet_functor_mt_allocator new_mt_allocator( pointer, sizeof( bullet_functor ) * CALLBACKS_PER_BULLET * m_max_bullets_count );
		m_mt_stack_allocator.swap			( new_mt_allocator );
	}



	m_bullets_memory_ptr				= new_bullets_memory_ptr;

	// STRUCTURE DIFF: target 31 / base 31 stmts
	// SIZE -0x3  | 416 | unmanaged_allocation_resource_ptr new_bullets_memory_ptr = static_cast_resource_ptr<unmanaged_allocation_resource_ptr>( queries[0].get_unmanaged_resource() );
	// SIZE +0x33 | 455 | m_mt_stack_allocator.swap			( new_mt_allocator );
	// VERDICT: STRUCTURE MATCH (shape ok) - cast row: target binds get_unmanaged_resource result through a ref temp, base consumes the prvalue directly (static_cast_resource_ptr by-value inline lowering, LTCG); swap row: base inlines std::swap<void*> where target calls the promoted out-of-line instantiation; both non-steerable.
}

// STATE[100%|DONE]
void bullet_manager::emit_bullet(
	float3 const&					position,
	float3 const&					velocity,
	const float						air_resistance,
	weapon_ammunition_ptr const&	wa,
	weapon_core const&				wc,
	const u32						current_time_in_ms,
	hit_initiator const* const		initiator,
	hit_receiver const* const		ignorable_object,
	bool							tracer
)
{
	if ( m_bullets_allocator_ref->total_size( ) == m_bullets_allocator_ref->allocated_size( ) )
		destroy_one_bullet( ); // sushi@NOTE: This will copy the whole array

	bullet* new_bullet = VOSTOK_NEW_IMPL( m_bullets_allocator_ref.c_ptr( ), bullet )( *this, position, velocity, current_time_in_ms, air_resistance, wa, wc, initiator, ignorable_object );
	ASSERT( UNKNOWN_EXPRESSION );

	if ( tracer && m_engine )
		m_engine->attach_tracer( new_bullet );

	m_bullets.push_back( new_bullet );
	ASSERT( UNKNOWN_EXPRESSION );

#ifndef MASTER_GOLD
	store_bullet_trajectory		( new_bullet );
#endif // #ifndef MASTER_GOLD
}

// STATE[99.89%|PARTIAL]: frame-size LTCG (target sub esp,40h vs base 2Ch).
void bullet_manager::destroy_bullet( buffer_vector<bullet*>::iterator const& destroying_bullet_iterator )
{
	bullet* destroying_bullet = *destroying_bullet_iterator;

	m_bullets.erase( destroying_bullet_iterator );
	VOSTOK_DELETE_IMPL( *m_bullets_allocator_ref, destroying_bullet );

	// STRUCTURE DIFF: target 3 / base 3 stmts (no diverging rows, 0x62 bytes both)
	// VERDICT: STRUCTURE MATCH - residual is target sub esp,40h vs base 2Ch + slot renames; LTCG stack lowering, non-steerable.
}

// STATE[100%|DONE]
void bullet_manager::destroy_one_bullet( )
{
	if ( !m_bullets.empty( ) )
		destroy_bullet( m_bullets.begin( ) );
}

// STATE[100%|DONE]
bool bullet_manager::is_inside_collision_db( float3 const& position ) const
{
	return m_physics_world->get_world_aabb( ).contains( position );
}

} // namespace survarium
