#include "TRACSInterface.h"
#include <thread>
#include <mutex>          // std::mutex


class TRACSLoop
{
	private:
		//int num_threads = 1;
		std::vector<TRACSInterface*> TRACSsim;
		std::vector<std::thread> t;
		std::mutex mtx, mtx_nt;           // mutex for critical sections
		std::string fnm;

	public:
		// Constructor
		TRACSLoop(std::string filename = "Config.TRACS", int nthreads = std::thread::hardware_concurrency()); // Reads values, initializes detector

		// Destructor
		~TRACSLoop();

		//functions
		void run();
		void call_from_thread(int tid);
};