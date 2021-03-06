#ifndef DXF_H
#define DXF_H
 
#include <vector>

#ifdef USE_DIME
#include "defined.h"
#endif

// Forward Declarations
namespace charutilities 
{
	class ijk ;
	class xyz ;
}



//namespace charutilities 
//{

bool readDxf (
	char const * const & aFileName,
	std::vector < charutilities::xyz > & aoVertices,		//must be allocated by: new type[0]
	std::vector < charutilities::ijk > & aoTriangles);		//must be allocated by: new type[0]
              
//} // charutilities

#endif // DXF_H 










