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

// claude@NOTE: the target's flash glue was compiled OPTIMIZED (the shipped
// value.cpp records "0 stmts / 1 byte" for the gfx() helpers and most getters,
// i.e. gfx() + the SF_INLINE GFx::Value chain were inlined AND emitted
// standalone, statement info collapsed). Our base builds /Od, so gfx() stays a
// real call (an extra statement) and every wrapper carries frame/call overhead.
// Structure is faithful (Call pairs 7/7 stmts, the ctors/dtors/SetElement pair
// clean SIZE-only); the byte/QUANTITY residual is that /Od-vs-optimized wall,
// not a source shape to steer.

namespace survarium {

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

flash_function_handler::~flash_function_handler( )
{
	if ( impl )
		delete impl;
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

flash_external_handler::~flash_external_handler( )
{
	if ( impl )
		delete impl;
}

void flash_movie::SetExternalInterface( flash_external_handler* handler )
{
	m_movie->SetExternalInterface( handler->impl );
}

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

void flash_movie::CreateFunction( flash_value* value, flash_function_handler* handler )
{
	m_movie->CreateFunction( gfx( value ), handler->impl );
}

void flash_movie::CreateObject( flash_value* value )
{
	m_movie->CreateObject( gfx( value ) );
}

void flash_movie::CreateArray( flash_value* value )
{
	m_movie->CreateArray( gfx( value ) );
}

bool flash_movie::Invoke( pcstr method_name, flash_value* result, flash_value const* args, u32 args_count )
{
	return m_movie->Invoke( method_name, gfx( result ), gfx( args ), args_count );
}

void flash_movie::GetVariable( flash_value* value, pcstr path )
{
	m_movie->GetVariable( gfx( value ), path );
}

void flash_movie::SetVariable( pcstr path, flash_value const& value )
{
	m_movie->SetVariable( path, gfx( value ) );
}

void flash_movie::SetVariable( pcstr path, pcstr value )
{
	m_movie->SetVariable( path, value );
}

u32 flash_value::GetUInt( ) const
{
	return gfx( this )->GetUInt( );
}

s32 flash_value::GetInt( ) const
{
	return gfx( this )->GetInt( );
}

wchar_t const* flash_value::GetStringW( ) const
{
	return gfx( this )->GetStringW( );
}

void flash_value::SetConvertStringW( )
{
	gfx( this )->SetConvertStringW( );
}

pcstr flash_value::GetString( ) const
{
	return gfx( this )->GetString( );
}

u32 flash_value::GetArraySize( )
{
	return gfx( this )->GetArraySize( );
}

void flash_value::GetElement( u32 index, flash_value* value )
{
	gfx( this )->GetElement( index, gfx( value ) );
}

void flash_value::PushBack( flash_value& value )
{
	gfx( this )->PushBack( gfx( value ) );
}

void flash_value::SetElement( u32 index, pcstr value )
{
	gfx( this )->SetElement( index, Scaleform::GFx::Value( value ) );
}

void flash_value::SetElement( u32 index, flash_value& value )
{
	gfx( this )->SetElement( index, gfx( value ) );
}

void flash_value::GetMember( pcstr member_name, flash_value* value )
{
	gfx( this )->GetMember( member_name, gfx( value ) );
}

bool flash_value::GetBool( )
{
	return gfx( this )->GetBool( );
}

float flash_value::GetNumber( )
{
	return ( float )gfx( this )->GetNumber( );
}

void flash_value::SetNumber( float value )
{
	gfx( this )->SetNumber( value );
}

void flash_value::SetUInt( u32 value )
{
	gfx( this )->SetUInt( value );
}

void flash_value::SetInt( s32 value )
{
	gfx( this )->SetInt( value );
}

void flash_value::SetStringW( wchar_t const* value )
{
	gfx( this )->SetStringW( value );
}

void flash_value::SetString( pcstr value )
{
	gfx( this )->SetString( value );
}

void flash_value::SetBoolean( bool value )
{
	gfx( this )->SetBoolean( value );
}

flash_value::~flash_value( )
{
	gfx( this )->~Value( );
}

flash_value::flash_value( )
{
	new ( body ) Scaleform::GFx::Value( );
}

void flash_value::SetMember( pcstr member_name, flash_value& value )
{
	gfx( this )->SetMember( member_name, gfx( value ) );
}

flash_function_handler_impl::flash_function_handler_impl( flash_function_handler& handler )
	:	owner	( handler )
{
}

flash_function_handler::flash_function_handler( )
{
	impl = SF_NEW flash_function_handler_impl( *this );
}

flash_external_handler::flash_external_handler( )
{
	impl = SF_NEW flash_external_handler_impl( *this );
}

} // namespace survarium
