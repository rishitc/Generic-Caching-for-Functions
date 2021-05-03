#pragma once

#include "lru_utility.hpp"
#include <list>


template<typename R, typename ...Args>
class lru_cache {
    // Creating type alias for complex types
    using Arguments = std::tuple<Args...>;
    using Values = std::pair<R, typename std::list<Arguments>::iterator>;

    // Creating type alias for complex types
    using cache_size_t = std::optional<std::size_t>;
    using cache_t = std::unordered_map<Arguments, Values, internal_cache::tuple_hasher>;
    using list_it_t = std::list<Arguments>::iterator;
    using list_const_it_t = std::list<Arguments>::const_iterator;

    public:
        explicit lru_cache(std::function<R(Args...)> f, cache_size_t cache_size = std::nullopt);
        //  :
        //     func_{f}, cache_size_{cache_size}, cache_{}, usage_tracker_{}
        // {
        //     if (cache_size.has_value())
        //         cache_.reserve(cache_size.value());
        // }

        R operator () (Args... arg_list);
        void flush_cache();


    private:
        std::function<R(Args...)> func_;
        cache_size_t cache_size_;
        cache_t cache_;
        std::list<Arguments> usage_tracker_;
};


template<typename R, typename ...Args>
lru_cache<R, Args...>::lru_cache(std::function<R(Args...)> f, cache_size_t cache_size = std::nullopt) :
    func_{f}, cache_size_{cache_size}, cache_{}, usage_tracker_{}
{
    if (cache_size_.has_value())
        cache_.reserve(cache_size_.value());
}

template<typename R, typename ...Args>
R lru_cache<R, Args...>::operator () (Args... arg_list)
{
    const auto tuple_ele = std::make_tuple(arg_list...);

    if (cache_.find(tuple_ele) != std::end(cache_)) {
        list_it_t &list_it_ = cache_[tuple_ele].second;
        std::cout << "Cache Hit!" << std::endl;
        usage_tracker_.erase(list_it_);
        usage_tracker_.push_back(tuple_ele);
        list_it_ = std::end(usage_tracker_);
    }
    else {
        // Eviction code
        if (cache_size_.has_value() && cache_.size() == cache_size_.value()) {
            list_const_it_t lru_const_it_ = std::begin(usage_tracker_);
            cache_.erase(*lru_const_it_);  // Eviction by Key
            usage_tracker_.erase(lru_const_it_);  // Eviction by Iterator
        }

        usage_tracker_.push_back(tuple_ele);
        cache_[tuple_ele] = Values{func_(arg_list...), std::end(usage_tracker_)};
        std::cout << "Cache Miss" << std::endl;
        std::cout << "Value added to cache" << std::endl;
    }

    return cache_[tuple_ele].first;
}

template<typename R, typename ...Args>
void lru_cache<R, Args...>::flush_cache()
{
    cache_.clear();  // Clear the cache
    usage_tracker_.clear();  // Clear the bookkeeping
}
