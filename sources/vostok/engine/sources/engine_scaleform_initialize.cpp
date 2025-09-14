////////////////////////////////////////////////////////////////////////////
//	Created		: 15.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "engine_world.h"

#include <vostok/os_preinclude.h>
#define _WIN32_WINNT				0x0500
#include <vostok/os_include.h>

#include <GFx.h>
#pragma comment( lib, "libgfx.lib" )
#pragma comment( lib, "libgfx_zlib.lib" )
#pragma comment( lib, "libgfx_libpng.lib" )
#pragma comment ( lib,"libgfx_as2.lib")
#pragma comment ( lib,"libgfx_as3.lib")
#pragma comment ( lib,"libgfx_libjpeg.lib" )

using Scaleform::UPInt;

class xrSysAllocMalloc : public Scaleform::SysAlloc
{
public:
    xrSysAllocMalloc() {}
	virtual ~xrSysAllocMalloc() {}


#if 0 //defined(SF_OS_WIN32)
    virtual void* Alloc(UPInt size, UPInt align)
    {
        return _aligned_malloc(size, align);
		//return VOSTOK_MALLOC_IMPL( vostok::memory::g_mt_allocator, size, "scaleform" );
    }

    virtual void  Free(void* ptr, UPInt size, UPInt align)
    {
        SF_UNUSED2(size, align);
        _aligned_free(ptr);
		//return VOSTOK_FREE_IMPL( vostok::debug::g_mt_allocator, ptr );
    }

    virtual void* Realloc(void* oldPtr, UPInt oldSize, UPInt newSize, UPInt align)
    {
        SF_UNUSED(oldSize);
        return _aligned_realloc(oldPtr, newSize, align);
		//return VOSTOK_REALLOC_IMPL( vostok::debug::g_mt_allocator, oldPtr, newSize, "scaleform");
    }
#else
	virtual void* Alloc(UPInt size, UPInt align)
    {
        //UPInt ptr = (UPInt)malloc(size+align);
		UPInt ptr = (UPInt)VOSTOK_MALLOC_IMPL( vostok::memory::g_mt_allocator, size+align, "scaleform" );
        UPInt aligned = 0;
        if (ptr)
        {
            aligned = (UPInt(ptr) + align-1) & ~(align-1);
            if (aligned == ptr) 
                aligned += align;
            *(((UPInt*)aligned)-1) = aligned-ptr;
        }
        return (void*)aligned;
    }

    virtual void  Free(void* ptr, UPInt size,  UPInt align)
    {
        UPInt src = UPInt(ptr) - *(((UPInt*)ptr)-1);
		void* p = (void*)src;
        VOSTOK_FREE_IMPL(vostok::memory::g_mt_allocator, p);
        SF_UNUSED2(size, align);
    }

    virtual void* Realloc(void* oldPtr, UPInt oldSize, UPInt newSize, UPInt align)
    {
        void* newPtr = Alloc(newSize, align);
        if (newPtr)
        {
            memcpy(newPtr, oldPtr, (newSize < oldSize) ? newSize : oldSize);
            Free(oldPtr, oldSize, align);
        }
        return newPtr;
    }
#endif
};

namespace vostok{
namespace engine{

void engine_world::initialize_scaleform( )
{
	static xrSysAllocMalloc scaleform_alloc;
	Scaleform::GFx::System::Init(&scaleform_alloc);
	LOG_INFO("engine_world::initialize_core gfx heap is %x", Scaleform::Memory::GetGlobalHeap());
}

void engine_world::destroy_scaleform( )
{
}

}}