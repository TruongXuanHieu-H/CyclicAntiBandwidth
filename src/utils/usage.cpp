#include <map>
#include <string>
#include <iostream>
#include <iomanip>

#include "usage.h"

const std::map<std::string, std::string> Helper::option_list = {
    {"--help", "Print usage message with all possible options"},
    {"--reduced", "Use reduced naive encoding for staircase constraints [default: false]"},
    {"--seq", "Use sequential encoding for staircase constraints [default: false]"},
    {"--product", "Use 2-Product encoding for staircase constraints [default: false]"},
    {"--duplex", "Use duplex encoding for staircase constraints [default: true]"},
    {"--ladder", "Use ladder encoding for staircase constraints and NSC for At-Most-One constraints [default: false]"},
    {"--conf-sat", "Use --sat configuration of CaDiCaL [default: true]"},
    {"--conf-unsat", "Use --unsat configuration of CaDiCaL [default: false]"},
    {"--conf-def", "Use default configuration of CaDiCaL [default: false]"},
    {"--force-phase", "Set options --forcephase,--phase=0 and --no-rephase of CaDiCal [default: false]"},
    {"--check-solution", "Calculate the antibandwidth of the found SAT solution and compare it to the actual width [default: false]"},
    {"--from-ub", "Start solving with width = UB, decreasing in each iteration [default: false]"},
    {"--from-lb", "Start solving with width = LB, increasing in each iteration [default: true]"},
    {"--bin-search", "Start solving with LB+UB/2 and update LB or UB according to SAT/UNSAT result and repeat"},
    {"-split-size <n>", "Maximal allowed length of clauses, every longer clause is split up into two by introducing a new variable"},
    {"-set-lb <new LB>", "Overwrite predefined LB with <new LB>, has to be at least 2"},
    {"-set-ub <new UB>", "Overwrite predefined UB with <new UB>, has to be positive"},
    {"-symmetry-break <break point>", "Apply symmetry breaking technique in <break point> (f: first node, h: highest degree node, l: lowest degree node, n: none) [default: none]"},
    {"-print-w <w>", "Only encode and print SAT formula of specified width w (where w > 0), without solving it"}};

void Helper::print_usage()
{
    std::cout << "usage: abw_enc path_to_graph_file/graph_file.mtx.rnd [ <option> ... ] " << std::endl;
    std::cout << "where '<option>' is one of the following options:" << std::endl;
    std::cout << std::endl;
    for (const auto &option : option_list)
    {
        std::cout << std::left << "\t" << std::setw(30) << option.first << "\t" << option.second << std::endl;
    }
    std::cout << std::endl;
}