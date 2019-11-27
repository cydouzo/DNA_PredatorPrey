#include <iostream>
#include <string>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/openmp/openmp.hpp>
#include <boost/timer/timer.hpp>

#include "constants.hpp"
#include "PdeSystem/predator_prey_systems.hpp"
#include "PDESystemCUDA/iterator_system.hpp"
#include "utilitary/initialConditionsBuilder.hpp"

using boost::timer::cpu_timer;
using namespace std;

const size_t nSpecies = 2, sampleSize = (size_t)(xLength / dh);
matrix x(nSpecies, sampleSize, 0.0);
matrix y(nSpecies, sampleSize, 0.0);

void initialization()
{
    int centerRabb = centerRabbRaw / dh;
    int widthRabb = widthRabbRaw / dh;
    int centerPred = centerPredRaw / dh;
    int widthPred = widthPredRaw / dh;

    gaussianMaker(x, 0, sampleSize, maxRabb, centerRabb, widthRabb);
    gaussianMaker(y, 0, sampleSize, maxRabb, centerRabb, widthRabb);
    gaussianMaker(x, 1, sampleSize, maxPred, centerPred, widthPred);
    gaussianMaker(y, 1, sampleSize, maxPred, centerPred, widthPred);
}

//Will run the program twice (once for CPU, once for GPU) and output the runtime for each
void CpuGpuCompare()
{
    cout << "Setup done, starting computation" << endl;

    cpu_timer timer; /* 
    integrate_const(runge_kutta4<matrix>(), prey_predator_system(1.2),
                    y, 0.0, tmax, dt); */
    double run_time = static_cast<double>(timer.elapsed().wall) * 1.0e-9;

    cpu_timer timer_gpu;
    Iterator_system iterator(x.data().begin(), nSpecies, sampleSize, t0, printPeriod);
    iterator.iterate(dt, tmax);
    double run_time_gpu = static_cast<double>(timer_gpu.elapsed().wall) * 1.0e-9;

    std::cout << "Ended computation in: " << endl;
    std::cout << " -Cpu Computation: " << run_time << "s" << std::endl;
    std::cout << " -GPU Computation: " << run_time_gpu << "s" << std::endl;

    std::cout << "Results saved in : \n"
              << iterator.outputPath << std::endl;

    std::ofstream fout("dataName");
    fout << iterator.dataName;
}

//Will output the runtime in seconds in the file performance/*dataName*
void PerformanceOriented(char arg)
{
    cpu_timer timer;

    string dataName = "cpu";
    if (arg == 'g')
    {
        Iterator_system iterator(x.data().begin(), nSpecies, sampleSize, t0, 0);
        iterator.iterate(dt, tmax);
        dataName = iterator.dataName;
    }
    else if (arg == 'c')
    {
        integrate_const(runge_kutta4<matrix>(), prey_predator_system(1.2),
                        x, 0.0, tmax, dt);
    }
    double run_time = static_cast<double>(timer.elapsed().wall) * 1.0e-9;
    std::cout /* << " -Computation Time: " */ << run_time << /* "s" <<  */ std::endl;

    std::ostringstream stream;
    stream << "performance/" << dataName;
    std::ofstream fout(stream.str(), std::ios_base::app);
    fout << run_time << std::endl;
}

int main(int argc, char **argv)
{
    /*     initialization();

    if (argc > 1)
    {
        char arg = argv[1][0];
        if (arg == 'c' || arg == 'g')
            PerformanceOriented(arg);
        else
            arg = 'g';
    }
    else
    {
        CpuGpuCompare();
    }
*/
    return 0;
}