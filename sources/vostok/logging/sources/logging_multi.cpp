////////////////////////////////////////////////////////////////////////////
//	Created		: 27.10.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "logging.h"

void vostok::logging::write_to_stdstream (stdstream_enum stream, pcstr format, ...)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( stream, format );
}

bool vostok::logging::initialize_console			( )
{
	return						false;
}

void vostok::logging::finalize_console			( )
{
}

void vostok::logging::initialize_stdstreams_if_needed ( )
{
}