////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "flash_external_handler.h"
#include "flash_external_handler_impl.h"
#include "flash_function_handler.h"
#include "flash_function_handler_impl.h"
#include "flash_function_handler_params.h"
#include "flash_movie.h"
#include "flash_value.h"

#include "GFx.h"

namespace survarium {

// STATE[STUB]
void flash_external_handler_impl::Callback(
		Scaleform::GFx::Movie*			movie,
		pcstr							method_name,
		Scaleform::GFx::Value const*	args,
		u32								args_count
	)
{
	// FUNCTION BODY[0x0ae920]
	VOSTOK_UNREFERENCED_PARAMETERS	( movie, method_name, args, args_count );
}

// STATE[STUB]
flash_function_handler::~flash_function_handler( )
{
	// FUNCTION BODY[0x5baac0]
}

// STATE[STUB]
void flash_function_handler_impl::Call( Scaleform::GFx::FunctionHandler::Params const& params )
{
	// FUNCTION BODY[0x5baae0]
	VOSTOK_UNREFERENCED_PARAMETER	( params );
}

// STATE[STUB]
flash_external_handler::~flash_external_handler( )
{
	// FUNCTION BODY[0x5bab30]
}

// STATE[STUB]
void flash_movie::SetExternalInterface( flash_external_handler* handler )
{
	// FUNCTION BODY[0x5bab50]
	VOSTOK_UNREFERENCED_PARAMETER	( handler );
}

// the flash_value <-> GFx::Value bridge helpers (addressed free functions;
// flash_value::body is the opaque GFx::Value storage)
// STATE[STUB]
Scaleform::GFx::Value* gfx( flash_value const* value )
{
	// FUNCTION BODY[0x5bab70]
	return ( Scaleform::GFx::Value* )( value->body );
}

// STATE[STUB]
Scaleform::GFx::Value& gfx( flash_value const& value )
{
	// FUNCTION BODY[0x5bab80]
	return *( Scaleform::GFx::Value* )( value.body );
}

// STATE[STUB]
void flash_movie::CreateFunction( flash_value* value, flash_function_handler* handler )
{
	// FUNCTION BODY[0x5bab90]
	VOSTOK_UNREFERENCED_PARAMETERS	( value, handler );
}

// STATE[STUB]
void flash_movie::CreateObject( flash_value* value )
{
	// FUNCTION BODY[0x5babb0]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
void flash_movie::CreateArray( flash_value* value )
{
	// FUNCTION BODY[0x5babc0]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
bool flash_movie::Invoke( pcstr method_name, flash_value* result, flash_value const* args, u32 args_count )
{
	// FUNCTION BODY[0x5babd0]
	VOSTOK_UNREFERENCED_PARAMETERS	( method_name, result, args, args_count );
	return false;
}

// STATE[STUB]
void flash_movie::GetVariable( flash_value* value, pcstr path )
{
	// FUNCTION BODY[0x5babf0]
	VOSTOK_UNREFERENCED_PARAMETERS	( value, path );
}

// STATE[STUB]
void flash_movie::SetVariable( pcstr path, flash_value const& value )
{
	// FUNCTION BODY[0x5bac10]
	VOSTOK_UNREFERENCED_PARAMETERS	( path, value );
}

// STATE[STUB]
void flash_movie::SetVariable( pcstr path, pcstr value )
{
	// FUNCTION BODY[0x5bac30]
	VOSTOK_UNREFERENCED_PARAMETERS	( path, value );
}

// STATE[STUB]
u32 flash_value::GetUInt( ) const
{
	// FUNCTION BODY[0x5bac90]
	return 0;
}

// STATE[STUB]
s32 flash_value::GetInt( ) const
{
	// FUNCTION BODY[0x5baca0]
	return 0;
}

// STATE[STUB]
wchar_t const* flash_value::GetStringW( ) const
{
	// FUNCTION BODY[0x5bacb0]
	return NULL;
}

// STATE[STUB]
void flash_value::SetConvertStringW( )
{
	// FUNCTION BODY[0x5bacc0]
}

// STATE[STUB]
pcstr flash_value::GetString( ) const
{
	// FUNCTION BODY[0x5bacf0]
	return NULL;
}

// STATE[STUB]
u32 flash_value::GetArraySize( )
{
	// FUNCTION BODY[0x5bad10]
	return 0;
}

// STATE[STUB]
void flash_value::GetElement( u32 index, flash_value* value )
{
	// FUNCTION BODY[0x5bad20]
	VOSTOK_UNREFERENCED_PARAMETERS	( index, value );
}

// STATE[STUB]
void flash_value::PushBack( flash_value& value )
{
	// FUNCTION BODY[0x5bad40]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
void flash_value::SetElement( u32 index, pcstr value )
{
	// FUNCTION BODY[0x5bad50]
	VOSTOK_UNREFERENCED_PARAMETERS	( index, value );
}

// STATE[STUB]
void flash_value::SetElement( u32 index, flash_value& value )
{
	// FUNCTION BODY[0x5badb0]
	VOSTOK_UNREFERENCED_PARAMETERS	( index, value );
}

// STATE[STUB]
void flash_value::GetMember( pcstr member_name, flash_value* value )
{
	// FUNCTION BODY[0x5badd0]
	VOSTOK_UNREFERENCED_PARAMETERS	( member_name, value );
}

// STATE[STUB]
bool flash_value::GetBool( )
{
	// FUNCTION BODY[0x5bae10]
	return false;
}

// STATE[STUB]
float flash_value::GetNumber( )
{
	// FUNCTION BODY[0x5bae20]
	return 0.f;
}

// STATE[STUB]
void flash_value::SetNumber( float value )
{
	// FUNCTION BODY[0x5bae30]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
void flash_value::SetUInt( u32 value )
{
	// FUNCTION BODY[0x5bae70]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
void flash_value::SetInt( s32 value )
{
	// FUNCTION BODY[0x5baea0]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
void flash_value::SetStringW( wchar_t const* value )
{
	// FUNCTION BODY[0x5baed0]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
void flash_value::SetString( pcstr value )
{
	// FUNCTION BODY[0x5baf30]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
void flash_value::SetBoolean( bool value )
{
	// FUNCTION BODY[0x5baf90]
	VOSTOK_UNREFERENCED_PARAMETER	( value );
}

// STATE[STUB]
flash_value::~flash_value( )
{
	// FUNCTION BODY[0x5bafd0]
}

// STATE[STUB]
flash_value::flash_value( )
{
	// FUNCTION BODY[0x5bb000]
}

// STATE[STUB]
void flash_value::SetMember( pcstr member_name, flash_value& value )
{
	// FUNCTION BODY[0x5bb020]
	VOSTOK_UNREFERENCED_PARAMETERS	( member_name, value );
}

// STATE[STUB]
flash_function_handler_impl::flash_function_handler_impl( flash_function_handler& handler )
	:	owner	( handler )
{
	// FUNCTION BODY[0x5bb060]
}

// STATE[STUB]
flash_function_handler::flash_function_handler( )
{
	// FUNCTION BODY[0x5bb080]
}

// STATE[STUB]
flash_external_handler::flash_external_handler( )
{
	// FUNCTION BODY[0x5bb0c0]
}

} // namespace survarium
