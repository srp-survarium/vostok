////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_VALUE_H_INCLUDED
#define FLASH_VALUE_H_INCLUDED

namespace survarium {

struct flash_value {
	inline						flash_value			( ) { /* no source */ }
	inline						~flash_value		( ) { /* no source */ }

	inline	void				SetBoolean			( bool arg_0 ) { /* no source */ }

	inline	void				SetString			( pcstr arg_0 ) { /* no source */ }

	inline	void				SetStringW			( wchar_t const* arg_0 ) { /* no source */ }

	inline	void				SetInt				( s32 arg_0 ) { /* no source */ }

	inline	void				SetUInt				( u32 arg_0 ) { /* no source */ }

	inline	void				SetNumber			( float arg_0 ) { /* no source */ }

	inline	void				SetMember			( pcstr arg_0, flash_value& arg_1 ) { /* no source */ }

	inline	void				SetElement			( u32 arg_0, flash_value& arg_1 ) { /* no source */ }
	inline	void				SetElement			( u32 arg_0, pcstr arg_1 ) { /* no source */ }

	inline	void				GetElement			( u32 arg_0, flash_value* arg_1 ) { /* no source */ }

	inline	void				PushBack			( flash_value& arg_0 ) { /* no source */ }

	inline	u32					GetArraySize		( ) { /* no source */ }

	inline	void				GetMember			( pcstr arg_0, flash_value* arg_1 ) { /* no source */ }

	inline	float				GetNumber			( ) { /* no source */ }

	inline	bool				GetBool				( ) { /* no source */ }

	inline	pcstr				GetString			( ) const { /* no source */ }

	inline	void				SetConvertStringW	( ) { /* no source */ }

	inline	wchar_t const*		GetStringW			( ) const { /* no source */ }

	inline	s32					GetInt				( ) const { /* no source */ }

	inline	u32					GetUInt				( ) const { /* no source */ }

public:
	/* 0x0000 */	char	body[24];
}; // struct flash_value

STATIC_SIZE_ASSERT(flash_value, 0x18);

} // namespace survarium

#endif // #ifndef FLASH_VALUE_H_INCLUDED
