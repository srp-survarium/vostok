// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef TEXT_TRANSLATOR_H_INCLUDED
#define TEXT_TRANSLATOR_H_INCLUDED

#include <vostok/configs.h>	// configs::binary_config_ptr (value member)

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok


namespace survarium {

class text_translator {
public:
			text_translator			( );
			~text_translator		( );

	void	translate_text			( pcstr text_id, wchar_t* translated_text );

	void	load_text_localization	( );

private:
	void	on_texts_ready			( resources::queries_result& data );

private:
	/* 0x0000 */	configs::binary_config_ptr		m_text_data;
}; // class text_translator

STATIC_SIZE_ASSERT(text_translator, 0x4);

} // namespace survarium

#endif // #ifndef TEXT_TRANSLATOR_H_INCLUDED
