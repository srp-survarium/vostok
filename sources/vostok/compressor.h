////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_COMPRESSOR_H_INCLUDED
#define VOSTOK_COMPRESSOR_H_INCLUDED

#include <vostok/memory_buffer.h>
#include <vostok/debug/signalling_bool.h>

namespace vostok {

class VOSTOK_CORE_API compressor
{
public:
	virtual vostok::signalling_bool	compress	(const_buffer src, mutable_buffer dest, u32& out_size) = 0;
	virtual vostok::signalling_bool	decompress	(const_buffer src, mutable_buffer dest, u32& out_size) = 0;

	virtual ~compressor () {}

}; // class compressor

} // namespace vostok

#endif // #ifndef VOSTOK_PPMD_ALLOCATOR_H_INCLUDED