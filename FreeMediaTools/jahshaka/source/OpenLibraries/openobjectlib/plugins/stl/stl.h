#ifndef STL_H
#define STL_H
 
#include <vector>

#ifdef WHEREFROM

#include "defined.h"

#endif

// Forward Declarations
namespace charutilities 
{
	class ijk ;
	class xyz ;
}



namespace charutilities 
{
//using namespace std ;

bool readStl (
	char const * const & aFileName,
	std::vector < xyz > & aoVertices,		//must be allocated by: new type[0]
	std::vector < ijk > & aoTriangles);		//must be allocated by: new type[0]
          
} // charutilities

#endif // STL_H 










