#include "cabw_instance.h"
#include "../global_data.h"
#include "instance_data.h"
#include "sat_solver_cadical.h"
#include "ladder_encoder.h"
#include <iostream>
#include <chrono>

CABWInstance::CABWInstance(int width)
{
    InstanceData::width = width;
}

CABWInstance::~CABWInstance() {}

/*
       Return the result of ABP:
       -   0 if graph only contains 1 vertex.
       -   10 if SAT (including w < 2 because it is always SAT).
       -   20 if UNSAT.
       -   -20 for undefined answers.
       -   -10 for incorrect SAT answers.
   */
int CABWInstance::encode_and_solve_cabp()
{
    std::cout << "c " + InstanceData::get_signature() + " Antibandwidth problem with w = " << InstanceData::width << " (" << GlobalData::g->graph_name << "):\n";
    if (GlobalData::g->n < 1)
    {
        std::cout << "c " + InstanceData::get_signature() + " The input graph is too small, there is nothing to encode here.\n";
        SAT_res = 0; // should break loop
        return 0;
    }
    if (InstanceData::width < 2)
    {
        std::cout << "c " + InstanceData::get_signature() + " There is always at least 1 distance in any labelling. There is nothing to encode here.\n";
        SAT_res = 10; // check solution can not be invoked
        return 10;
    }

    InstanceData::setup_for_solving();
    std::cout << "c " + InstanceData::get_signature() + " Encoding starts with w = " << InstanceData::width << ":\n";

    auto t1 = std::chrono::high_resolution_clock::now();
    InstanceData::enc->encode_cyclic_antibandwidth();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto encode_duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();

    std::cout << "c " + InstanceData::get_signature() + " Encoding duration: " << encode_duration << "s\n";
    std::cout << "c " + InstanceData::get_signature() + " Number of clauses: " << InstanceData::cc->size() << std::endl;
    std::cout << "c " + InstanceData::get_signature() + " Number of variables: " << InstanceData::vh->size() << std::endl;
    std::cout << "c " + InstanceData::get_signature() + " SAT Solving starts:\n";

    t1 = std::chrono::high_resolution_clock::now();
    SAT_res = InstanceData::solver->solve();
    t2 = std::chrono::high_resolution_clock::now();
    auto solving_duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "c " + InstanceData::get_signature() + " Solving duration: " << solving_duration << " ms\n";
    std::cout << "c " + InstanceData::get_signature() + " Answer: \n";
    if (SAT_res == 10)
    {
        std::cout << "s " + InstanceData::get_signature() + " SAT (w = " << InstanceData::width << ")\n";
    }
    else if (SAT_res == 20)
        std::cout << "s " + InstanceData::get_signature() + " UNSAT (w = " << InstanceData::width << ")\n";
    else
    {
        std::cout << "s " + InstanceData::get_signature() + " Error at w = " << InstanceData::width << ", SAT result: " << SAT_res << std::endl;
        InstanceData::cleanup_solving();
        return -20;
    }

    if (GlobalData::enable_solution_verification && SAT_res == 10)
    {
        int solution_abp = verify_solution();
        if (solution_abp < InstanceData::width)
        {
            std::cerr << "c " + InstanceData::get_signature() + " Error, the solution is not correct, antibandwidth should be at least " << InstanceData::width << ", but it is " << solution_abp << ".\n";

            InstanceData::cleanup_solving();
            return -10;
        }
        else if (solution_abp == InstanceData::width)
        {
            std::cout << "c " + InstanceData::get_signature() + " The solution is correct.\n";
        }
        else
        {
            std::cout << "c " + InstanceData::get_signature() + " Found an optimal solution " << solution_abp << ".\n ";
        }
    }

    InstanceData::cleanup_solving();

    // std::cout << "c " + get_signature() + " Closed.\n";
    return SAT_res;
};

int CABWInstance::verify_solution()
{
    std::vector<int> node_labels = InstanceData::solver->extract_result();
    if ((int)node_labels.size() == 0)
    {
        return 0;
    }
    int min_dist = GlobalData::g->calculate_cyclic_antibandwidth(node_labels);

    std::cout << "c " + InstanceData::get_signature() + " Solution check = " << min_dist << ".\n";

    return min_dist;
}
