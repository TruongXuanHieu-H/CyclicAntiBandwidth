#ifndef SEARCH_STRATEGY_H
#define SEARCH_STRATEGY_H

enum class SearchStrategy
{
    iterate_from_lb, // Search from lower bound iteratively
    step_from_lb,    // Search from lower bound with steps
};

#endif // SEARCH_STRATEGY_H