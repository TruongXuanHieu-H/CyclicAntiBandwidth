#ifndef CABW_ENCODER_H
#define CABW_ENCODER_H

#include "enum/encode_type.h"
#include "enum/search_strategy.h"
#include "graph/graph.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <limits>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <chrono>
#include <fstream>

class CyclicAntiBandwidthEncoder
{
public:
    CyclicAntiBandwidthEncoder();
    virtual ~CyclicAntiBandwidthEncoder();

    Graph *g = nullptr;
    int worker_count = 1;

    int max_width_SAT = std::numeric_limits<int>::min();
    int min_width_UNSAT = std::numeric_limits<int>::max();

    EncodeType encode_type = EncodeType::Seq;
    SearchStrategy search_strategy = SearchStrategy::from_lb;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;

    bool verbose = true;
    bool enable_solution_verification = false;
    int split_limit = 0;
    std::string symmetry_break_point = "n";

    bool overwrite_lb = false;
    bool overwrite_ub = false;
    int forced_lb = 0;
    int forced_ub = 0;

    void read_graph(std::string graph_file_name);
    void encode_and_solve();
    void encode_and_print_dimacs(int width);

    void create_limit_pid();
    void create_cabp_pid(int width);
    int do_cabp_pid_task(int width);

    int sample_rate = 100000; // Interval of sampler, in microseconds
    int report_rate = 100;    // Interval of report, in number of sampler
    int sampler_count = 0;

    float memory_limit = std::numeric_limits<float>::max();       // bound of total memory consumed by all the processes, in megabyte
    float real_time_limit = std::numeric_limits<float>::max();    // bound of time consumed by main process, in seconds
    float elapsed_time_limit = std::numeric_limits<float>::max(); // bound of total time consumed by all the process, in seconds

    float *max_consumed_memory;
    float consumed_memory = 0;       // total memory consumed by all the processes, in megabyte
    float consumed_real_time = 0;    // time consumed by main process, in seconds
    float consumed_elapsed_time = 0; // total time consumed by all the process, in seconds

private:
    std::unordered_map<int, pid_t> abp_pids;
    pid_t lim_pid;

    void encode_and_solve_from_lb();

    void encode_and_solve_with_widths(int start_width, int step, int stop_width);
    void encode_and_solve_with_width(int width);

    void lookup_bounds(int &lb, int &ub);
    void setup_bounds(int &w_from, int &w_to);

    int is_limit_satisfied();

    static std::unordered_map<std::string, int> cabw_LBs;
    static std::unordered_map<std::string, int> cabw_UBs;
};

#endif // CABW_ENCODER_H