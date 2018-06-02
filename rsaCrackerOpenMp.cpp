#include <iostream>
#include <thread>
#include <omp.h>
#include <boost/math/special_functions/prime.hpp>


const bool verbose = false;

int main() {
    int p = boost::math::prime(30);
    int q = boost::math::prime(100);
    int n = p * q;
    std::cout << "Key to crack: " << n << std::endl;
    std::cout << "Answer is: " << p << " and " << q << std::endl;

    int maxPCounter = 1000;
    std::cout << "omp_get_max_threads: " << omp_get_max_threads() << std::endl;
    int pChunkSize = maxPCounter / omp_get_max_threads();

    int resultP = 0, resultQ = 0;
#pragma omp parallel firstprivate(n, pChunkSize) shared(resultP, resultQ)
    {
        int foundP, foundQ, counterP, counterQ;

        int i = omp_get_thread_num();
        int startingP = i * pChunkSize;
        int maxCounterP = (i + 1) * pChunkSize;

        counterP = startingP;
        counterQ = counterP + 1;
        foundP = boost::math::prime(counterP);
        foundQ = boost::math::prime(counterQ);
        while (counterP <= maxCounterP && foundP * foundQ != n) {
            if (verbose) {
                std::cout << "I am process: " << omp_get_thread_num() << std::endl;
                std::cout << "counterP = " << counterP << " counterQ = " << counterQ << std::endl;
            }
            if (foundQ >= n / foundP) {
                counterP++;
                counterQ = counterP + 1;
            } else {
                counterQ++;
            }
            foundP = boost::math::prime(counterP);
            foundQ = boost::math::prime(counterQ);
        }
        if (foundP * foundQ == n) {
            resultQ = foundQ;
            resultP = foundP;
        }
        if (verbose) {
            std::cout << "Answer: " << "counterP = " << counterP << " counterQ = " << counterQ << std::endl;
            std::cout << "Answer: " << "foundP = " << foundP << " foundQ = " << foundQ << std::endl;
        }
    }
    if (resultP == 0 || resultQ == 0) {
        std::cout << "No answer found !" << std::endl;
    } else {
        std::cout << "Answer: " << "resultP = " << resultP << " resultQ = " << resultQ << std::endl;
    }
    return 0;
}