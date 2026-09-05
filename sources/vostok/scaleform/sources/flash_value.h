// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLASH_VALUE_H_INCLUDED
#define FLASH_VALUE_H_INCLUDED
#include <vostok/scaleform/api.h>

namespace survarium {

// opaque storage for a Scaleform::GFx::Value (0x18 bytes), keeping the SDK
// type out of the public header
struct VOSTOK_SCALEFORM_API flash_value {
								flash_value			( );
								~flash_value		( );

			void				SetBoolean			( bool arg_0 );

			void				SetString			( pcstr arg_0 );

			void				SetStringW			( wchar_t const* arg_0 );

			void				SetInt				( s32 arg_0 );

			void				SetUInt				( u32 arg_0 );

			void				SetNumber			( float arg_0 );

			void				SetMember			( pcstr arg_0, flash_value& arg_1 );

			void				SetElement			( u32 arg_0, flash_value& arg_1 );
			void				SetElement			( u32 arg_0, pcstr arg_1 );

			void				GetElement			( u32 arg_0, flash_value* arg_1 );

			void				PushBack			( flash_value& arg_0 );

			u32					GetArraySize		( );

			void				GetMember			( pcstr arg_0, flash_value* arg_1 );

			float				GetNumber			( );

			bool				GetBool				( );

			pcstr				GetString			( ) const;

			void				SetConvertStringW	( );

			wchar_t const*		GetStringW			( ) const;

			s32					GetInt				( ) const;

			u32					GetUInt				( ) const;

public:
	/* 0x0000 */	char	body[24];
}; // struct flash_value

STATIC_SIZE_ASSERT(flash_value, 0x18);

} // namespace survarium

#endif // #ifndef FLASH_VALUE_H_INCLUDED
