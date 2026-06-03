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

// STATE[UNCHECKED]
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

	// FUNCTION BODY[0x5a28d0]: 4
	// <0x5a29f3>|0x123|+0x008:'45'
	// <0x5a29fb>|0x12b|+0x008:'46'
	// <0>
	// <1>
	// ******
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
	// STATE[90.15%|DONE] LTCG for float3
	inline	explicit	redundant_bullet_predicate	( bullet_manager& bullet_manager ) : bullet_manager( &bullet_manager ) { }

			bool		operator()					( bullet* bullet )
	{
		if ( bullet->get_start_velocity( ) != float3( 0, 0, 0 ) )
			return false;

		bullet_manager->free_bullet( bullet );
		return true;

		// FUNCTION BODY[0xbe610]: 5
		// <0xbe619>|0x009|+0x02e:'86'
		// <0xbe647>|0x037|+0x004:'87'
		// <0>
		// <0xbe64b>|0x03b|+0x00e:'89'
		// <0xbe659>|0x049|+0x002:'90'
		// ******
	}

public:
	/* 0x0000 */	survarium::bullet_manager*		bullet_manager;
}; // struct redundant_bullet_predicate

// STATE[87.80%|PARTIAL]: `delete_helper` didn't inline in target.
void bullet_manager::free_bullet( bullet* bullet )
{
	if ( m_engine && bullet->m_tracer_idx != u16(-1) )
		m_engine->detach_tracer( bullet );

	VOSTOK_DELETE_IMPL( *m_bullets_allocator_ref, bullet );

	// FUNCTION BODY[0x5a13f0]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a13f9>|0x009|+0x018:'103'
	// <0x5a1411>|0x021|+0x017:'104'
	// <0>
	// <0x5a1428>|0x038|+0x025:'106'
	// ******
}

// STATE[98.49%|DONE]: Stack variables allocated slightly differently, since `boost::function` Got LTCG'd differently (ecx vs eax / ecx vs esi).
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

// STATE[94.92%|DONE]
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
		m_functors.push( functor );	// sushi@MATCH: Seems the same!
	}

	// FUNCTION BODY[0x5a1980]: 13
	// <0x5a1990>|0x010|+0x020:'152'
	// <0>
	// <0x5a19b0>|0x030|+0x04d:'154'
	// <0>
	// <0x5a19fd>|0x07d|+0x00f:'156'
	// <0x5a1a0c>|0x08c|+0x019:'157'
	// <0x5a1a25>|0x0a5|+0x019:'158'
	// <0x5a1a3e>|0x0be|+0x019:'159'
	// <0x5a1a57>|0x0d7|+0x00d:'160'
	// <0x5a1a64>|0x0e4|+0x009:'161'
	// <0x5a1a6d>|0x0ed|+0x067:'162'
	// <0x5a1ad4>|0x154|+0x0b6:'163'
	// <0>
	// ******
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

// STATE[67.87%|PARTIAL]: bind problems
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
			functor->resource,
			functor->position,
			functor->direction,
			functor->normal
		);
		m_functors.push( functor );
	}

	// FUNCTION BODY[0x5a16e0]: 13
	// <0x5a16f0>|0x010|+0x010:'217'
	// <0>
	// <0x5a1700>|0x020|+0x049|[1]:'219'
	// <0>
	// <0x5a1749>|0x069|+0x00f:'221'
	// <0x5a1758>|0x078|+0x019:'222'		functor->position = position;
	// <0x5a1771>|0x091|+0x019:'223'		functor->direction = direction;
	// <0x5a178a>|0x0aa|+0x00b:'224'		functor->direction.normalize( );
	// <0x5a1795>|0x0b5|+0x019:'225'		functor->direction = normal;
	// <0x5a17ae>|0x0ce|+0x00b:'226'		functor->normal.normalize( );
	// <0x5a17b9>|0x0d9|+0x103:'227'
	// <0x5a18bc>|0x1dc|+0x0bc:'228'
	// <0>
	// ******
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

// STATE[58.56%|PARTIAL] sushi@TODO: functor->position & functor->direction were pushed differently
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
			functor->position,
			functor->direction,
			length
		);
		m_functors.push( functor );
	}

	// FUNCTION BODY[0x5a14a0]: 15
	// <0x5a14b0>|0x010|+0x010:'239'
	// <0>
	// < >|0x020|+0x04d:'241'
	// <0>
	// <1>
	// <0x5a150d>|0x06d|+0x019:'244'
	// <0x5a1526>|0x086|+0x019:'245'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a153f>|0x09f|+0x0da:'251'
	// <0x5a1619>|0x179|+0x0bc:'252'
	// <0>
	// ******
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

// STATE[92.30%:PARTIAL]: Lot's of LTCG diffs. Maybe something more
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
}

// STATE[93.67%|BLOCKED]: I am just tired matching this.
// sushi@NOTE:
//	* static_cast_resource_ptr for casting unmanaged resources. Also see what IDA generated to match similar cases in the future.
//  * See how destructors for '}' are run
void bullet_manager::bullets_memory_allocated( resources::queries_result& queries )
{   // sushi@TODO: Note that not all memory is currently being used. Specifically, yes: particle, decal, update_tracer ; me: sound ; no: attach_tracer, detach_tracer

	R_ASSERT								( queries[0].is_successful( ) );
	unmanaged_allocation_resource_ptr new_bullets_memory_ptr = static_cast_resource_ptr<unmanaged_allocation_resource_ptr>( queries[0].get_unmanaged_resource() );
	{	// sushi@TODO: ???
		pbyte pointer = new_bullets_memory_ptr->buffer;

		bool is_realocation = m_bullets_allocator_ref.initialized( ) && m_bullets_allocator_ref->allocated_size( ) > 0;
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

			VOSTOK_CONSTRUCT_REFERENCE		( m_bullets_allocator_ref, bullets_allocator )( new_bullets_memory_ptr->buffer, m_max_bullets_count * sizeof( bullet ) ); // sushi@MATCH: Target has some strange call

			pointer							+= m_max_bullets_count * sizeof( bullet );
			bullets_type new_bullets_list	( pointer, m_max_bullets_count );
			m_bullets.swap					( new_bullets_list );
		}

		pointer								+= m_max_bullets_count * sizeof( bullet* );
		bullet_manager::bullet_functor_mt_allocator new_mt_allocator( pointer, sizeof( bullet_functor ) * CALLBACKS_PER_BULLET * m_max_bullets_count );
		m_mt_stack_allocator.swap			( new_mt_allocator );
	}



	m_bullets_memory_ptr				= new_bullets_memory_ptr;

	// FUNCTION BODY: 47
	// <0>
	// <0x5a1fb0>|0x010|+0x00c:'379'
	// <0x5a1fbc>|0x01c|+0x034:'380'		unmanaged_allocation_resource_ptr new_bullets_memory_ptr = static_cast_resource_ptr<unmanaged_allocation_resource_ptr>( queries[0].get_unmanaged_resource() );
	// <0>
	// <0x5a1ff0>|0x050|+0x017|[1]:'382'	pbyte pointer = new_bullets_memory_ptr->buffer;
	// <0>
	// <0x5a2007>|0x067|+0x055:'384'		bool is_realocation = m_bullets_allocator_ref.initialized( ) && m_bullets_allocator_ref->allocated_size( ) > 0;
	// <0x5a205c>|0x0bc|+0x00c:'385'		if ( is_realocation )
	// <0>
	// <0x5a2068>|0x0c8|+0x046|[3]:'387'		for ( u32 i = m_bullets_allocator_ref->allocated_size( ) / ( sizeof( bullet ) + sizeof( bullet* ) ); i > m_max_bullets_count; --i )
	// <0x5a20ae>|0x10e|+0x00d:'388'				destroy_one_bullet			( );
	// <0>
	// <0x5a20bb>|0x11b|+0x019:'390'			bullets_allocator new_bullets_allocator		= bullets_allocator( new_bullets_memory_ptr->buffer, m_max_bullets_count * sizeof( bullet ) );
	// <0>
	// <0x5a20d4>|0x134|+0x012:'392'			pointer							+= m_max_bullets_count * sizeof( bullet );
	// <0x5a20e6>|0x146|+0x032:'393'			bullets_type new_bullets_list	( pointer, m_max_bullets_count );
	// <0>
	// <0x5a2118>|0x178|+0x021:'395'			while ( !m_bullets.empty( ) ) {
	// <0>
	// <0x5a2139>|0x199|+0x025|[4]:'397'			bullet*  old_bullet			= m_bullets[0];
	// <0x5a215e>|0x1be|+0x082:'398'				new_bullets_list.push_back	( VOSTOK_NEW_IMPL( new_bullets_allocator, bullet ) ( *old_bullet ) );
	// <0x5a21e0>|0x240|+0x043:'399'				VOSTOK_DELETE_IMPL			( *m_bullets_allocator_ref, old_bullet );
	// <0x5a2223>|0x283|+0x005:'400'			}
	// <0>
	// <0x5a2228>|0x288|+0x030:'402'			new_bullets_allocator.swap		( *m_bullets_allocator_ref );
	// <0x5a2258>|0x2b8|+0x066:'403'			m_bullets.swap					( new_bullets_list );
	// <0x5a22be>|0x31e|+0x035:'404'		}
	// <0x5a22f3>|0x353|+0x005:'405'		else
	// <0>									{
	// <0x5a22f8>|0x358|+0x017|[2]:'407'		if( m_bullets_allocator_ref.initialized( ) )
	// <0x5a230f>|0x36f|+0x00e:'408'				VOSTOK_DESTROY_REFERENCE		( m_bullets_allocator_ref );
	// <0>
	// <0x5a231d>|0x37d|+0x09a:'410'			VOSTOK_CONSTRUCT_REFERENCE		( m_bullets_allocator_ref, bullets_allocator )( new_bullets_memory_ptr->buffer, m_max_bullets_count * sizeof( bullet ) );
	// <0>
	// <0x5a23b7>|0x417|+0x012:'412'			pointer							+= m_max_bullets_count * sizeof( bullet );
	// <0x5a23c9>|0x429|+0x032:'413'			bullets_type new_bullets_list	( pointer, m_max_bullets_count );
	// <0x5a23fb>|0x45b|+0x066:'414'			m_bullets.swap					( new_bullets_list );
	// <0x5a2461>|0x4c1|+0x02d:'415'		}
	// <0>
	// <0x5a248e>|0x4ee|+0x012:'417'		pointer								+= m_max_bullets_count * sizeof( bullet* );
	// <0x5a24a0>|0x500|+0x01c:'418'		bullet_manager::bullet_functor_mt_allocator new_mt_allocator( pointer, 0x210 * m_max_bullets_count );
	// <0x5a24bc>|0x51c|+0x033:'419'		m_mt_stack_allocator.swap			( new_mt_allocator );
	// <0x5a24ef>|0x54f|+0x008:'420'		// ??? }
	// <0>
	// <1>
	// <2>
	// <0x5a24f7>|0x557|+0x012:'424'
	// ******
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

// STATE[99.89%|DONE]: Allocated less stack space
void bullet_manager::destroy_bullet( buffer_vector<bullet*>::iterator const& destroying_bullet_iterator )
{
	bullet* destroying_bullet = *destroying_bullet_iterator;

	m_bullets.erase( destroying_bullet_iterator );
	VOSTOK_DELETE_IMPL( *m_bullets_allocator_ref, destroying_bullet );
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
