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

namespace SpeedTree {

// Defined by SpeedTreeCore (Core.obj); declared here because the SDK headers
// are not part of the repo.
extern CAllocator*	g_pAllocator;
extern size_t		g_siHeapMemoryUsed;
extern size_t		g_siNumHeapAllocs;

// CInstance ctor/dtor now come from SpeedTreeForest_v5.2 (Instance.obj);
// the carcass-era minimal stand-ins were removed when the SDK libs were linked.

} // namespace SpeedTree

namespace vostok {
namespace render {

using namespace SpeedTree;

// claude@NOTE: target proves namespace scope - `vostok::render::'dynamic initializer
// for 's_speedtree_cook''` [0x7c5a80] / 's_speedtree_instance_cook' [0x7c5a90].
static vostok::uninitialized_reference< vostok::render::speedtree_cook >				s_speedtree_cook;
static vostok::uninitialized_reference< vostok::render::speedtree_instance_cook >	s_speedtree_instance_cook;

void* speed_tree_allocator::Alloc( size_t size )
{
	// FUNCTION BODY[0x54440]
	// claude@NOTE: legacy also accumulated a file-static num_speedtree_memory_used and
	// threw std::bad_alloc on failure; the canonical carcass has neither the counter nor
	// exception handling enabled - matcher-phase work against the 0x4c-byte target body.
	return MALLOC( size, "speed_tree_allocator" );
}

void speed_tree_allocator::Free( void* block )
{
	// FUNCTION BODY[0x54490]
	if ( block )
		FREE( block );
}

void initialize_speedtree( )
{
	// FUNCTION BODY[0x75a880]
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
	// FUNCTION BODY[0x75aa10]
	resources::unregister_cook	( resources::speedtree_class );
	VOSTOK_DESTROY_REFERENCE	( s_speedtree_cook );

	resources::unregister_cook	( resources::speedtree_instance_class );
	VOSTOK_DESTROY_REFERENCE	( s_speedtree_instance_cook );
}

void print_speedtree_errors( )
{
	// FUNCTION BODY[0x75a910]
	const char* pError = SpeedTree::CCore::GetError( );
	while ( pError )
	{
		LOG_ERROR( "SpeedTree Error: %s\n", pError );
		pError = CCore::GetError( );
	}
}

} // namespace render
} // namespace vostok
