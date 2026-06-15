////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SCALEFORM_ENGINE_H_INCLUDED
#define SCALEFORM_ENGINE_H_INCLUDED

#include "Kernel/SF_SysAlloc.h"	// Scaleform::SysAlloc (base)

namespace survarium {

// scaleform_engine is a NAMESPACE (initialize/destroy mangle as free
// functions); all the addressed bodies live in factory.cpp.
namespace scaleform_engine {

void	initialize	(
			void* ( *alloc )( u32 ),
			void ( *free )( void* ),
			void ( *log )( u8, pcstr )
		);
void	destroy		( );

// X-Ray-era allocator hook handed to Scaleform::GFx::System::Init
class xrSysAllocMalloc : public Scaleform::SysAlloc {
public:
	inline				xrSysAllocMalloc	( void* ( *arg_0 )( u32 ), void ( *arg_1 )( void* ) ) { /* no source */ }
	virtual				~xrSysAllocMalloc	( ) { /* no source */ }

	// the three SysAlloc overrides are out-of-line in factory.cpp per the rich index
	virtual	void*		Alloc				( u32 arg_0, u32 arg_1 ) override;

	virtual	void		Free				( void* arg_0, u32 arg_1, u32 arg_2 ) override;

	virtual	void*		Realloc				(
							void*		arg_0,
							u32			arg_1,
							u32			arg_2,
							u32			arg_3
						) override;

private:
	/* 0x0000 */	/* Scaleform::SysAlloc */
	/* 0x0004 */	void* ( *m_mem_alloc_ptr )( u32 );
	/* 0x0008 */	void ( *m_mem_free_ptr )( void* );
}; // class xrSysAllocMalloc

} // namespace scaleform_engine

STATIC_SIZE_ASSERT(scaleform_engine::xrSysAllocMalloc, 0xC);

} // namespace survarium

#endif // #ifndef SCALEFORM_ENGINE_H_INCLUDED
