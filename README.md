# simple-rsa-cracker
Simple RSA cracker.

This repo includes cracking RSA in paralel way using 2 different C++ libraries:
* OpenMP
* Boost.MPI

### How to compile code and then run it

First you should have installed dependencies on you PC:
In Ubuntu case it would be:
  
  
* **OpenMPI:**

        sudo apt-get install openmpi-bin openmpi-common libopenmpi1.10 libopenmpi-dev
  
* **Boost:**

        sudo apt-get install libboost-all-dev
  
 Next in projects root create target folder for cmake:
 
    mkdir target 
    
Then go into it and execute cmake:
    
    cd target
    cmake ../
    
Finally build sources calling:
     
     make
     
OpenMP executable you can run as any regular executable:

    ./RSA_Cracker_OpenMp
     
But to run Boost.MPI in parallel mode you have to launch it using mpirun and pass number of proccesses in params:
 
    mpirun -np 4 RSA_Cracker_BoostMpi