#include "affect_event_type_enum.h"
#include "affects_applying_type_enum.h"
#include "hit_affects_type_enum.h"

#include "affects_threshold.h"
#include "body_part_parameters.h"
#include "damage_model.h"
#include "damage_protector.h"
#include "hit_type_parameters.h"

namespace stalker2
{

class IncludeAll
{
public:
	IncludeAll();
};


class Callback1
{
public:
	Callback1();

	void complete ( char const * name );
	void partial  ( char const * name, int value );

public:
	typedef boost::function< void ( char const * ) > fn;
	
	fn m_complete;
	fn m_partial;
};

class Callback2
{
public:
	Callback2();

	void complete ( char const * name, int value );
	void partial  ( char const * name, int value, float precision );

public:
	typedef boost::function< void ( char const *, int ) > fn;	

	fn m_complete;
	fn m_partial;
};

}