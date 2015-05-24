#ifndef REGLAGES_H
#define REGLAGES_H

#if PIC_BUILD
#	if   GROS
#		include "reglages/gros_robot.h"
#	elif PETIT
#		include "reglages/petit_robot.h"
#	endif
#else
#	include "reglages/PC.h"
#endif

#endif
