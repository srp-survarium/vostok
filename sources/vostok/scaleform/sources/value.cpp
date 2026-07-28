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

// claude@NOTE: this TU now builds /Ox (Master Gold Optimization=3, since the
// scaleform vcproj flip), so the trivial setters/getters/ctors/dtors byte-match
// the target. The remaining partials are NOT /Od artifacts; they are genuine /Ox
// residuals over already-faithful structure:
//  - Call pairs 5/5 statements at the exact target lines, same 0x46 total bytes;
//    the only delta is the optimizer interleaving the params.pMovie->GetUserData()
//    load across the field-store statements (scheduling, not source-steerable).
//  - SetVariable/GetVariable/SetElement pair clean (gfx() inlines); the byte
//    residual is per-statement instruction scheduling of the inlined GFx::Value.
//  - SetString/SetStringW: the target source is `*gfx(this) = GFx::Value(value)`
//    (constructs a typed Value temp, inlines operator=). Our /Ox does NOT inline
//    GFx::Value::operator= (emits `call operator=` + an explicit temp ~Value),
//    which scores WORSE (unpaired) than the SF_INLINE SetString form kept here.
//    The temp+operator= form is what the target wrote; restore it once the SDK
//    operator= inlines (or it is forced inline). Kept at the paired SetString form.
//  - GetMember pairs against the wrong base COMDAT (objdiff folds it onto a
//    DrawText helper that pushes the "text" literal + ret 4) - a pairing artifact,
//    not a source gap; our GetMember(member_name, value) is correct.
//  - Callback: 2 target statements (movie->GetUserData() evaluated first, then the
//    virtual owner.callback) vs our base's right-to-left arg push; the target has
//    0 named locals, so the early GetUserData is the optimizer's arg scheduling for
//    the virtual call, not a named-temp source shape we can add.

namespace survarium {

// the flash_value <-> GFx::Value bridge helpers (addressed free functions;
// flash_value::body is the opaque GFx::Value storage)
Scaleform::GFx::Value* gfx( flash_value const* value )
{
	return ( Scaleform::GFx::Value* )( value->body );
}

Scaleform::GFx::Value& gfx( flash_value const& value )
{
	return *( Scaleform::GFx::Value* )( value.body );
}

flash_value::flash_value( )
{
	new ( body ) Scaleform::GFx::Value( );
}

flash_value::~flash_value( )
{
	gfx( this )->~Value( );
}

void flash_value::SetBoolean( bool value )
{
	gfx( this )->SetBoolean( value );
}

void flash_value::SetString( pcstr value )
{
	gfx( this )->SetString( value );
}

void flash_value::SetStringW( wchar_t const* value )
{
	gfx( this )->SetStringW( value );
}

void flash_value::SetInt( s32 value )
{
	gfx( this )->SetInt( value );
}

void flash_value::SetUInt( u32 value )
{
	gfx( this )->SetUInt( value );
}

void flash_value::SetNumber( float value )
{
	gfx( this )->SetNumber( value );
}

float flash_value::GetNumber( )
{
	return ( float )gfx( this )->GetNumber( );
}

bool flash_value::GetBool( )
{
	return gfx( this )->GetBool( );
}

void flash_value::SetMember( pcstr member_name, flash_value& value )
{
	gfx( this )->SetMember( member_name, gfx( value ) );
}

void flash_value::GetMember( pcstr member_name, flash_value* value )
{
	gfx( this )->GetMember( member_name, gfx( value ) );
}

void flash_value::SetElement( u32 index, flash_value& value )
{
	gfx( this )->SetElement( index, gfx( value ) );
}

void flash_value::SetElement( u32 index, pcstr value )
{
	gfx( this )->SetElement( index, Scaleform::GFx::Value( value ) );
}

void flash_value::PushBack( flash_value& value )
{
	gfx( this )->PushBack( gfx( value ) );
}

void flash_value::GetElement( u32 index, flash_value* value )
{
	gfx( this )->GetElement( index, gfx( value ) );
}

u32 flash_value::GetArraySize( )
{
	return gfx( this )->GetArraySize( );
}

pcstr flash_value::GetString( ) const
{
	return gfx( this )->GetString( );
}

void flash_value::SetConvertStringW( )
{
	gfx( this )->SetConvertStringW( );
}

wchar_t const* flash_value::GetStringW( ) const
{
	return gfx( this )->GetStringW( );
}

s32 flash_value::GetInt( ) const
{
	return gfx( this )->GetInt( );
}

u32 flash_value::GetUInt( ) const
{
	return gfx( this )->GetUInt( );
}

void flash_movie::SetVariable( pcstr path, pcstr value )
{
	m_movie->SetVariable( path, value );
}

void flash_movie::SetVariable( pcstr path, flash_value const& value )
{
	m_movie->SetVariable( path, gfx( value ) );
}

void flash_movie::GetVariable( flash_value* value, pcstr path )
{
	m_movie->GetVariable( gfx( value ), path );
}

void flash_external_handler_impl::Callback(
		Scaleform::GFx::Movie*			movie,
		pcstr							method_name,
		Scaleform::GFx::Value const*	args,
		u32								args_count
	)
{
	owner.callback(
		( flash_movie* )movie->GetUserData( ),
		method_name,
		( flash_value const* )args,
		args_count
	);
}

bool flash_movie::Invoke( pcstr method_name, flash_value* result, flash_value const* args, u32 args_count )
{
	return m_movie->Invoke( method_name, gfx( result ), gfx( args ), args_count );
}

void flash_movie::CreateArray( flash_value* value )
{
	m_movie->CreateArray( gfx( value ) );
}

void flash_movie::CreateObject( flash_value* value )
{
	m_movie->CreateObject( gfx( value ) );
}

void flash_movie::SetExternalInterface( flash_external_handler* handler )
{
	m_movie->SetExternalInterface( handler->impl );
}

void flash_movie::CreateFunction( flash_value* value, flash_function_handler* handler )
{
	m_movie->CreateFunction( gfx( value ), handler->impl );
}

flash_external_handler::flash_external_handler( )
{
	impl = SF_NEW flash_external_handler_impl( *this );
}

flash_external_handler::~flash_external_handler( )
{
	if ( impl )
		delete impl;
}

flash_function_handler_impl::flash_function_handler_impl( flash_function_handler& handler )
	:	owner	( handler )
{
}

void flash_function_handler_impl::Call( Scaleform::GFx::FunctionHandler::Params const& params )
{
	flash_function_handler_params	p;
	p.pRetVal	= ( flash_value* )params.pRetVal;
	p.pArgs		= ( flash_value* )params.pArgs;
	p.pMovie	= ( flash_movie* )params.pMovie->GetUserData( );
	p.ArgCount	= params.ArgCount;

	owner.call( p );
}

flash_function_handler::flash_function_handler( )
{
	impl = SF_NEW flash_function_handler_impl( *this );
}

flash_function_handler::~flash_function_handler( )
{
	if ( impl )
		delete impl;
}

} // namespace survarium
