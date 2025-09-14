/**************************************************************************

PublicHeader:   Kernel
Filename    :   HeapMH_SysAllocMalloc.h
Content     :   System Allocator Interface
Created     :   2009
Authors     :   Maxim Shemanarev

Notes       :   Interface to the system allocator.

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_Kernel_HeapMH_SysAllocMalloc_H
#define INC_SF_Kernel_HeapMH_SysAllocMalloc_H

#include "../SF_SysAlloc.h"

#if defined(SF_OS_WIN32) || defined(SF_OS_WINCE) || defined(SF_OS_XBOX) || defined(SF_OS_XBOX360)
#include <malloc.h>
#elif defined(SF_OS_WII)
#include <string.h>
#elif defined(SF_OS_3DS)
#include <nn/os.h>
#include <stdlib.h>
#include <string.h>
#else
#include <stdlib.h>
#include <string.h>
#endif

namespace Scaleform {

class SysAllocMalloc : public SysAllocBase_SingletonSupport<SysAllocMalloc, SysAlloc>
{
public:
#if defined(SF_OS_WII)
    // Amount of memory to reserve in MEM2 arena; 0 is all available space
    SysAllocMalloc(UPInt size = 0)
    {
        UByte *m2arenaLo = (UByte*) OSGetMEM2ArenaLo();
        UByte *m2arenaHi = (UByte*) OSGetMEM2ArenaHi();
        if (size > 0 && (m2arenaLo + size < m2arenaHi))
            m2arenaHi = m2arenaLo + size;
        OSSetMEM2ArenaLo(m2arenaHi);
        //SF_DEBUG_WARNING(m2arenaHi-m2arenaLo < 65536, "GSysAllocMalloc: Less than 64k memory available");

        ::new(PrivateData) SysAllocStatic(m2arenaLo, (UByte*)m2arenaHi-(UByte*)m2arenaLo);
    }
#elif defined(SF_OS_3DS)
    SysAllocMalloc(UByte* base = 0, UPInt size = 17 << 20);
#else
    SysAllocMalloc() {}
#endif
    virtual ~SysAllocMalloc() {}

#if defined(SF_OS_WIN32) || defined(SF_OS_WINCE) || defined(SF_OS_XBOX) || defined(SF_OS_XBOX360)
    virtual void* Alloc(UPInt size, UPInt /*align*/)
    {
        //return _aligned_malloc(size, align);
		//return XRAY_MALLOC_IMPL( xray::memory::g_mt_allocator, size, "scaleform" );
		return XRAY_MALLOC_IMPL( xray::debug::g_mt_allocator, size, "scaleform" );
    }

    virtual void  Free(void* ptr, UPInt size, UPInt align)
    {
        SF_UNUSED2(size, align);
        //_aligned_free(ptr);
		return XRAY_FREE_IMPL( xray::debug::g_mt_allocator, ptr );
    }

    virtual void* Realloc(void* oldPtr, UPInt oldSize, UPInt newSize, UPInt /*align*/)
    {
        SF_UNUSED(oldSize);
        //return _aligned_realloc(oldPtr, newSize, align);
		return XRAY_REALLOC_IMPL( xray::debug::g_mt_allocator, oldPtr, newSize, "scaleform");
    }
#elif defined(SF_OS_PS3)
    virtual void* Alloc(UPInt size, UPInt align)
    {
        return memalign(align, size);
    }

    virtual void  Free(void* ptr, UPInt size, UPInt align)
    {
        SF_UNUSED2(size, align);
        free(ptr);
    }

    virtual void* Realloc(void* oldPtr, UPInt oldSize, UPInt newSize, UPInt align)
    {
        SF_UNUSED(oldSize);
        return reallocalign(oldPtr, newSize, align);
    }

#elif defined(SF_OS_WII) || defined(SF_OS_3DS)
    virtual void* Alloc(UPInt size, UPInt align)
    {
        return GetAllocator()->Alloc(size, align);
    }

    virtual void  Free(void* ptr, UPInt size, UPInt align)
    {
        GetAllocator()->Free(ptr, size, align);
    }

    virtual void* Realloc(void* oldPtr, UPInt oldSize, UPInt newSize, UPInt align)
    {
        void* newPtr = NULL;
        if (GetAllocator()->ReallocInPlace(oldPtr, oldSize, newSize, align))
            newPtr = oldPtr;
        else {
            newPtr = GetAllocator()->Alloc(newSize, align);
            if (newPtr)
            {
                memcpy(newPtr, oldPtr, (newSize < oldSize) ? newSize : oldSize);
                GetAllocator()->Free(oldPtr, oldSize, align);
            }
        }
        return newPtr;
    }

private:
    UPInt PrivateData[(sizeof(SysAllocStatic) + sizeof(UPInt)) / sizeof(UPInt)];
    SysAllocStatic* GetAllocator() { return (SysAllocStatic*) PrivateData; }

#else
    virtual void* Alloc(UPInt size, UPInt align)
    {
        UPInt ptr = (UPInt)malloc(size+align);
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
        free((void*)src);
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


} // Scaleform

#endif
