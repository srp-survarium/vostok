////////////////////////////////////////////////////////////////////////////
//	Created 	: 19.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MEMORY_READER_ABSTRACT_H_INCLUDED
#define VOSTOK_MEMORY_READER_ABSTRACT_H_INCLUDED

namespace vostok {
namespace memory {

class VOSTOK_CORE_API reader_abstract {
public:
	virtual	void	r	( pvoid& buffer, u32 size ) = 0;
}; // class reader_abstract

} // namespace memory
} // namespace vostok

#endif // #ifndef VOSTOK_MEMORY_READER_ABSTRACT_H_INCLUDED