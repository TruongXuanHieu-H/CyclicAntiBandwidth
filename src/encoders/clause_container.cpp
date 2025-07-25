#include "clause_container.h"
#include "../global_data.h"
#include "instance_data.h"
#include <iostream>

ClauseContainer::ClauseContainer()
{
    if (GlobalData::split_limit > 0)
    {
        do_split = true;
    }
    else
    {
        do_split = false;
    }
};

ClauseContainer::~ClauseContainer() {};

void ClauseContainer::add_clause(const Clause &c)
{
    if (!do_split)
    {
        do_add_clause(c);
    }
    else
    {
        Clause long_clause = c;
        while ((int)long_clause.size() > GlobalData::split_limit)
        {
            int split_var = InstanceData::vh->get_new_var();

            Clause chunk(long_clause.begin(), long_clause.begin() + GlobalData::split_limit);
            chunk.push_back(split_var);
            do_add_clause(chunk);

            Clause rest = {-1 * split_var};
            rest.insert(rest.end(), long_clause.begin() + GlobalData::split_limit, long_clause.end());
            long_clause = rest;
        }
        do_add_clause(long_clause);
    }
};

void ClauseContainer::print_clauses()
{
    do_print_clauses();
};

void ClauseContainer::print_dimacs()
{
    do_print_dimacs();
};

int ClauseContainer::size()
{
    return do_size();
};

void ClauseContainer::do_add_clause(const Clause &c)
{
    clause_list.push_back(c);
    // for (auto cl : c)
    // {
    //     std::cout << cl << " ";
    // }
    // std::cout << std::endl;

    if (InstanceData::solver)
        InstanceData::solver->add_clause(c);
};

void ClauseContainer::do_print_dimacs()
{
    std::cout << "p cnf " << InstanceData::vh->size() << " " << size() << std::endl;
    for (auto const &c : clause_list)
    {
        for (auto const &l : c)
        {
            std::cout << l << " ";
        }
        std::cout << "0" << std::endl;
    }
};

void ClauseContainer::do_print_clauses()
{
    for (auto const &c : clause_list)
    {
        for (auto const &l : c)
        {
            std::cout << l << " ";
        }
        std::cout << "0" << std::endl;
    }
};

int ClauseContainer::do_size()
{
    return clause_list.size();
};

void ClauseContainer::do_clear()
{
    clause_list.clear();
};
