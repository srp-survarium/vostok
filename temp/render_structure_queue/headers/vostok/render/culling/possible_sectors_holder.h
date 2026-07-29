////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_POSSIBLE_SECTORS_HOLDER_H_INCLUDED
#define RENDER_CULLING_POSSIBLE_SECTORS_HOLDER_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::configs::binary_config_value;

namespace vostok {
namespace render {
namespace culling {

class possible_sectors_holder : public core::noncopyable {
public:
	inline	explicit		possible_sectors_holder	( configs::binary_config_value arg_0 ) { /* no source */ }
	inline					~possible_sectors_holder( ) { /* no source */ }

	inline	bool			is_possible_sector		( u16 arg_0 ) const { /* no source */ }

	inline	u16 const*		begin					( ) const { /* no source */ }

	inline	u16 const*		end						( ) const { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	u16*	m_buffer;
	/* 0x0004 */	u16*	m_buffer_end;
}; // class possible_sectors_holder

STATIC_SIZE_ASSERT(possible_sectors_holder, 0x8);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_POSSIBLE_SECTORS_HOLDER_H_INCLUDED
