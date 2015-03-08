#ifndef DEBUG
#define DEBUG

// Include STL
#include <iostream>

#include "const.H"


#define PRINT std::cout
#define END_PRINT DEFAULT<<std::endl

#define PRINT_RED(Enonce)\
 	{\
 		PRINT<<RED<<"[ERROR]"<<Enonce<<END_PRINT;\
 	}
#define PRINT_GREEN(Enonce)\
 	{\
 		std::cout<<GREEN<<"[CONFIRMATION]"<<Enonce<<END_PRINT;\
 	} 	
#define PRINT_ORANGE(Enonce)\
 	{\
 		std::cout<<ORANGE<<"[MESSAGE]"<<Enonce<<END_PRINT;\
 	} 	
 #define PRINT_MAGENTA(Enonce)\
 	{\
 		std::cout<<ORANGE<<"[SYNCRONISATION]"<<Enonce<<END_PRINT;\
 	} 	


#endif // DEBUG