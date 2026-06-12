////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SCALEFORM_ENGINE_XRSYSALLOCMALLOC_H_INCLUDED
#define SCALEFORM_ENGINE_XRSYSALLOCMALLOC_H_INCLUDED

/* INCLUDES */
class Scaleform::SysAlloc;

namespace survarium {

class scaleform_engine::xrSysAllocMalloc : public Scaleform::SysAlloc {
public:
	inline				xrSysAllocMalloc	( void* ( * )( u32 ) arg_0, void ( * )( void* ) arg_1 ) { /* no source */ }
	virtual				~xrSysAllocMalloc	( ) { /* no source */ }

	virtual	void*		Alloc				( u32 arg_0, u32 arg_1 ) override { /* no source */ }

	virtual	void		Free				( void* arg_0, u32 arg_1, u32 arg_2 ) override { /* no source */ }

	virtual	void*		Realloc				(
							void*		arg_0,
							u32			arg_1,
							u32			arg_2,
							u32			arg_3
						) override { /* no source */ }

private:
	/* 0x0000 */	/* Scaleform::SysAlloc */
	/* 0x0004 */	void* ( * )( u32 )		m_mem_alloc_ptr;
	/* 0x0008 */	void ( * )( void* )		m_mem_free_ptr;
}; // class scaleform_engine::xrSysAllocMalloc

STATIC_SIZE_ASSERT(scaleform_engine::xrSysAllocMalloc, 0xC);

} // namespace survarium

#endif // #ifndef SCALEFORM_ENGINE_XRSYSALLOCMALLOC_H_INCLUDED
