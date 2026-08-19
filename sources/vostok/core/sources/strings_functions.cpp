////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/strings_functions.h>

namespace vostok {
namespace strings {

bool   convert_string_to_number (pcstr const string, float * const out_result)
{
	CURE_ASSERT					(string, return false, "1st argument is null pointer");
	CURE_ASSERT					(out_result, return false, "2nd argument is null pointer");

	float const number_value	= (float)atof( (pstr)string );
	* out_result				= number_value;
	if ( number_value != 0.f )
		return					true;

	return
		strings::equal(string, "0")	|| 
		strings::equal(string, "0.") || 
		strings::equal(string, "0.0");
}

bool   compare_with_wildcards (pcstr wild, pcstr string) 
{
    pcstr	cp = 0, mp = 0;

    while ( (*string) && (*wild != '*') ) 
	{
		if ( (*wild != *string) && (*wild != '?') ) 
            return		0;

        ++wild;
        ++string;
    }

	while ( *string )
	{
        if ( *wild == '*' ) 
		{
            if ( !*++wild ) 
				return 1;

            mp		=	wild;
            cp		=	string + 1;
        }
		else if ( (*wild == *string) || (*wild == '?') ) 
		{
            ++wild;
            ++string;
        } 
		else 
		{
            wild	=	mp;
            string	=	cp++;
        }
    }

	while ( *wild == '*' )
		++wild;

	return				!*wild;
}

bool set_multibyte ( pstr dest, const u32 dest_size_bytes, wchar_t const* src )
{
	VOSTOK_UNREFERENCED_PARAMETERS( dest, src );

	return dest_size_bytes == STRUNCATE;
}

bool mbstowcs ( wchar_t* dest, u32 dest_size_bytes, pcstr src )
{
	u32 converted_chars = 0;

	s32 const result = mbstowcs_s( &converted_chars, dest, dest_size_bytes / sizeof(wchar_t), src, _TRUNCATE );

	return result == 0 || result == STRUNCATE;
}

} // namespace vostok
} // namespace strings
