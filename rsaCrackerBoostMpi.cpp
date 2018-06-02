#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/math/special_functions/prime.hpp>
#include <boost/mpi/collectives.hpp>
#include <thread>


namespace mpi = boost::mpi;

const bool verbose = false;

int main() {
    mpi::environment env;
    mpi::communicator world;


    if (world.rank() == 0) {
        int p = boost::math::prime(30);
        int q = boost::math::prime(100);
        int n = p * q;
        std::cout << "Key to crack: " << n << std::endl;
        std::cout << "Answer is: " << p << " and " << q << std::endl;


        std::cout << "Boost process count = " << world.size() << std::endl;
        std::cout << "core count = " << std::thread::hardware_concurrency() << std::endl;

        if (world.size() < 2) {
            std::cerr << "Can't execute method, boost processes count is less then 2" << std::endl;
            return 1;
        }

        int maxPCounter = 1000;
        int pChunkSize = maxPCounter / world.size();

        for (int i = 1; i < world.size(); ++i) {
            world.send(i, 1, n);
            world.send(i, 2, (i - 1) * pChunkSize);
            world.send(i, 3, i * pChunkSize);
        }

        int resultP = 0;
        int resultQ = 0;

        for (int i = 1; i < world.size() && (resultQ == 0 || resultP == 0); ++i) {
            world.recv(mpi::any_source, 1, resultP);
            world.recv(mpi::any_source, 2, resultQ);
        }
        if (resultQ == 0 || resultP == 0) {
            std::cout << "No answer found !" << std::endl;
        } else {
            std::cout << "Answer: " << "resultP = " << resultP << " resultQ = " << resultQ << std::endl;
        }
    } else {
        int foundP, foundQ, counterP, counterQ;

        int startingP, maxCounterP;
        int n;
        world.recv(0, 1, n);
        world.recv(0, 2, startingP);
        world.recv(0, 3, maxCounterP);

        counterP = startingP;
        counterQ = counterP + 1;
        foundP = boost::math::prime(counterP);
        foundQ = boost::math::prime(counterQ);
        while (counterP <= maxCounterP && foundP * foundQ != n) {
            if (verbose) {
                std::cout << "I am process: " << world.rank() << std::endl;
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
            world.send(0, 1, foundP);
            world.send(0, 2, foundQ);
        } else {
            world.send(0, 1, 0);
            world.send(0, 2, 0);
        }
        if (verbose) {
            std::cout << "Answer: " << "counterP = " << counterP << " counterQ = " << counterQ << std::endl;
            std::cout << "Answer: " << "foundP = " << foundP << " foundQ = " << foundQ << std::endl;
        }
    }
    return 0;
}
