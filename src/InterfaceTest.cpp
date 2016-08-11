#include "TRACSInterface.h"
//#include <iostream>
 #include <thread>
#include <mutex>          // std::mutex
#include "global.h"


std::string fnm="Config.TRACS";

     

int main()
{
	
	TRACSInterface *TRACSsim = new TRACSInterface(fnm, 6); //CORRECT
	TRACSsim->loop_on();

	return 0;
	
}


