#include "TRACSLoop.h"
#include "global.h"



	


	TRACSLoop::TRACSLoop(std::string filename, int nthreads)
	{
			std::cout << "Number of cores = " << std::thread::hardware_concurrency() <<std::endl;
			fnm = filename;
			num_threads = nthreads;
	}


	// Destructor
	TRACSLoop::~TRACSLoop()
	{
	//	delete i_ramo, i_rc, i_conv;
	}

	//runs the multiple threads
	void TRACSLoop::run()
	{
		if(num_threads == 0){
		std::cout << "Error reading number of cores! Setting num_threads = 4"  <<std::endl;
		num_threads = 4;
		}
		TRACSsim.resize(num_threads);
		t.resize(num_threads);

         //Launch a group of threads
         for (int i = 0; i < num_threads; i++) {
         	if(i==0)
         		mtx_nt.lock();
         	if(i==1)
         		t.resize(num_threads);
             t[i] = std::thread(&TRACSLoop::call_from_thread, this, i);
         }
 
         std::cout << "Launched from the main\n";
 
         //Join the threads with the main thread
         for (int i = 0; i < num_threads; i++) {
             t[i].join();
         }
    //write output to single file!
    TRACSsim[0]->write_to_file(0);

    //getter test
    std::vector<double> neff_test = TRACSsim[0]->get_NeffParam();
    std::cout << "Neff param.: " << std::endl;
    for (int i = 0; i < 8; i++)
    {
    	    std::cout << neff_test[i] << std::endl;

    }


	}


//This function will be called from a thread
  
      void TRACSLoop::call_from_thread(int tid) {
        // every thread instantiates a new TRACSInterface object
       	 mtx.lock();
        if(tid < num_threads) // In case we reduced the number of threads!
        {	
	      	std::cout << "Thread" << tid << std::endl;
	      	TRACSsim[tid] = new TRACSInterface(fnm);
	      	TRACSsim[tid]->set_tcount(tid);
	      	if(tid==0)
	      	{
	      		i_ramo_array.clear();
				//i_conv_array.clear();
	      		TRACSsim[tid]->resize_array();
	      		TRACSsim[tid]->write_header(tid);
	      		TRACSsim.resize(num_threads);
	      		//t.resize(num_threads);
	      		mtx_nt.unlock();
	      	}
	       	//TRACSsim[tid]->write_header(tid);
		    std::cout << "Made it t" << tid << std::endl;
		    mtx.unlock();
		    TRACSsim[tid]->loop_on(tid);
		    /*
		      	 if(tid==0){
		      	 		std::string fnm1="Config.TRACS";
		      	 	std::cout << "Thread1!" << std::endl;
		      	 	 mtx.lock();
		      	 	TRACSInterface *TRACSsim = new TRACSInterface(fnm1);
		      	 	std::cout << "Made it t1!" << std::endl;
		      	 	 mtx.unlock();
		         	TRACSsim->loop_on(1);
		      	 }
		      	 if(tid==1){
		      	 	std::string fnm2="Config.TRACS";
		      	 	std::cout << "Thread2!" << std::endl;
		      	 	//for(int i=0; i<100000000; i++);
		      	 	mtx.lock();
		      	 	TRACSInterface *TRACSsim2 = new TRACSInterface(fnm2);
		      	 	std::cout << "Made it t2!" << std::endl;
					mtx.unlock();
		      	 	TRACSsim2->loop_on(2);
		      	 }
		    */  
	    	}	 
	    else
	    {
	    	t[tid].detach();
	    	mtx.unlock();
	    }
      	 }
