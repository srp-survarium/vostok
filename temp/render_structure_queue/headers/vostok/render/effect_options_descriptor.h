////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_OPTIONS_DESCRIPTOR_H_INCLUDED
#define RENDER_EFFECT_OPTIONS_DESCRIPTOR_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_options_descriptor::const_iterator;

namespace vostok {
namespace render {

struct effect_options_descriptor : public boost::noncopyable {
											effect_options_descriptor	( );
											effect_options_descriptor	( pcvoid in_data, u16 in_size );
											~effect_options_descriptor	( );

			u32								get_crc						( ) const;
			u32								get_data_memory_usage		( u32& need_bytes_to_align4, u32& last_align_value ) const;
			u32								get_num_total_fields		( ) const;
			u32								get_num_used_bytes			( ) const;

			effect_options_descriptor&		operator[]					( pcstr key );

	inline	void							call_destructors			( ) { /* no source */ }

	inline	effect_options_descriptor::const_iterator	begin						( ) const { /* no source */ }

	inline	effect_options_descriptor::const_iterator	end							( ) const { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	pcstr		id;
	/* 0x0004 */	pcvoid		destroyer;
	/* 0x0008 */	pbyte		data;
	/* 0x000c */	u32			bytes;
	/* 0x0010 */	u16			type;
	/* 0x0012 */	u16			count;
	/* 0x0014 */	u16			memory_size;
}; // struct effect_options_descriptor

STATIC_SIZE_ASSERT(effect_options_descriptor, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_OPTIONS_DESCRIPTOR_H_INCLUDED
