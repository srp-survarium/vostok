#include "pch.h"
#include "speedtree.h"

namespace SpeedTree {

// @todo Replace these minimal definitions with the SpeedTree 5.2 runtime library.
CAllocator*	g_pAllocator		= 0;
size_t		g_siHeapMemoryUsed	= 0;
size_t		g_siNumHeapAllocs	= 0;

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
