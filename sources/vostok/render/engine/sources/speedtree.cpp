#include "pch.h"
#include "speedtree.h"

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

CInstance::CInstance( ) :
	m_vPos					( ),
	m_fScale				( 1.f ),
	m_vGeometricCenter		( ),
	m_fCullingRadius		( 0.f ),
	m_nRotation				( 0 )
{
	m_anRotationVector[0]	= 0;
	m_anRotationVector[1]	= 0;
}

CInstance::~CInstance( )
{
}

} // namespace SpeedTree

namespace vostok {
namespace render {

void* speed_tree_allocator::Alloc( size_t size )
{
	return MALLOC( size, "speed_tree_allocator" );
}

void speed_tree_allocator::Free( void* block )
{
	if ( block )
		FREE( block );
}

void initialize_speedtree( )
{
	// @todo Recover the remaining SpeedTree runtime initialization.
	static speed_tree_allocator allocator;
	SpeedTree::g_pAllocator = &allocator;
}

void finalize_speedtree( )
{
	// @todo Recover the remaining SpeedTree runtime shutdown.
	SpeedTree::g_pAllocator = 0;
}

void print_speedtree_errors( )
{
	// @todo Recover SpeedTree SDK error reporting.
}

} // namespace render
} // namespace vostok
