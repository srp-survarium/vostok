////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef TEXT_TRANSLATOR_H_INCLUDED
#define TEXT_TRANSLATOR_H_INCLUDED

/* INCLUDES */
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::resources::queries_result;

namespace survarium {

class text_translator {
public:
			text_translator			( );
			~text_translator		( );

	void	translate_text			( pcstr text_id, wchar_t* translated_text );

	void	load_text_localization	( );

	void	on_texts_ready			( resources::queries_result& data );

private:
	/* 0x0000 */	configs::binary_config_ptr		m_text_data;
}; // class text_translator

STATIC_SIZE_ASSERT(text_translator, 0x4);

} // namespace survarium

#endif // #ifndef TEXT_TRANSLATOR_H_INCLUDED
