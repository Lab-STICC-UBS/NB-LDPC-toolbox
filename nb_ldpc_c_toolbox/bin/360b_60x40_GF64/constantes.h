#ifndef CONSTANTES
#define CONSTANTES

#include <math.h>

#define _GF_        		 64
#define _LOG2GF_    		  6

#define _KSYMBOL_   		 20
#define _NSYMBOL_   		 60
#define _MESSAGE_   		120

#define _DEG_CN_               3
#define _DEG_COMPUTATIONS_    40

#define _KBIT_      		(_KSYMBOL_*_LOG2GF_)
#define _NBIT_      		(_NSYMBOL_*_LOG2GF_)

#define _NmKSYMBOL_ 	 	(_NSYMBOL_-_KSYMBOL_)
#define _NmKBIT_     		(_NBIT_-_KBIT_)

#endif
