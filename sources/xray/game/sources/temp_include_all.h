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

	void partial( char const * name );

public:
	typedef boost::function< void ( char const * ) > fn1;
	
	fn1 m_fn;
};

}