////////////////////////////////////////////////////////////////////////////
//	Created		: 11.02.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef CONFIG_READER_H_INCLUDED
#define CONFIG_READER_H_INCLUDED

#include <vostok/configs.h>

namespace vostok {

namespace resources {
	class queries_result;
} // namespace resources

namespace core {
namespace configs {

class binary_config : public vostok::configs::binary_config {
public:
					binary_config	( pcbyte buffer, u32 buffer_size, vostok::memory::base_allocator* allocator );
	virtual		   ~binary_config	( );

	vostok::memory::base_allocator*	get_allocator() const { return m_allocator; }
private:
			void	load			( pcbyte buffer, u32 buffer_size );

private:
	vostok::memory::base_allocator*	m_allocator;
};

} // namespace configs
} // namespace core
} // namespace vostok

#endif // #ifndef CONFIG_READER_H_INCLUDED