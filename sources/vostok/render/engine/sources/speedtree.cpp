// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "speedtree.h"
#include "speedtree_cook.h"
#if VOSTOK_PLATFORM_WINDOWS_32
#	if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MT_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MT_Static.lib" )
#	else // #endif // #if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MTDLL_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MTDLL_Static.lib" )
#	endif // #if VOSTOK_STATIC_LIBRARIES
#else // #if VOSTOK_PLATFORM_WINDOWS_32
#	if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MT64_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MT64_Static.lib" )
#	else // #endif // #if VOSTOK_STATIC_LIBRARIES
#		pragma comment( lib, "SpeedTreeCore_v5.2_VC90MTDLL64_Static.lib" )
#		pragma comment( lib, "SpeedTreeForest_v5.2_VC90MTDLL64_Static.lib" )
#	endif // #if VOSTOK_STATIC_LIBRARIES
#endif // #if VOSTOK_PLATFORM_WINDOWS_32

static u32 num_speedtree_memory_used = 0;

namespace SpeedTree {

// Defined by SpeedTreeCore (Core.obj); declared here because the SDK headers
// are not part of the repo.
extern CAllocator*	g_pAllocator;
extern size_t		g_siHeapMemoryUsed;
extern size_t		g_siNumHeapAllocs;

} // namespace SpeedTree

namespace vostok {
namespace render {

using namespace SpeedTree;

void* speed_tree_allocator::Alloc( size_t size )
{
	void* block = MALLOC( size, "speed_tree_allocator" );

	num_speedtree_memory_used += size;

	if ( block == 0 )
		throw std::bad_alloc( );
	else
		return block;
}

void speed_tree_allocator::Free( void* block )
{
	if ( block )
		FREE( block );
}

static vostok::uninitialized_reference< vostok::render::speedtree_cook >				s_speedtree_cook;
static vostok::uninitialized_reference< vostok::render::speedtree_instance_cook >	s_speedtree_instance_cook;

void initialize_speedtree( )
{
	static speed_tree_allocator s_speed_tree_allocator;

	if ( !CCore::IsAuthorized( ) )
	{
		ASSERT( 0, "The SpeedTree SDK has not been authorized; expired evaluation key?" );
	}

	CAllocatorInterface cOn		( &s_speed_tree_allocator );

	CCoordSys::SetCoordSys		( CCoordSys::COORD_SYS_LEFT_HANDED_Y_UP );

	VOSTOK_CONSTRUCT_REFERENCE	( s_speedtree_cook, speedtree_cook );
	resources::register_cook	( s_speedtree_cook.c_ptr( ) );

	VOSTOK_CONSTRUCT_REFERENCE	( s_speedtree_instance_cook, speedtree_instance_cook );
	resources::register_cook	( s_speedtree_instance_cook.c_ptr( ) );
}

void finalize_speedtree( )
{
	resources::unregister_cook	( resources::speedtree_class );
	VOSTOK_DESTROY_REFERENCE	( s_speedtree_cook );

	resources::unregister_cook	( resources::speedtree_instance_class );
	VOSTOK_DESTROY_REFERENCE	( s_speedtree_instance_cook );
}

void print_speedtree_errors( )
{
	const char* pError = SpeedTree::CCore::GetError( );
	while ( pError )
	{
		LOG_ERROR( "SpeedTree Error: %s\n", pError );
		pError = CCore::GetError( );
	}
}

} // namespace render
} // namespace vostok
