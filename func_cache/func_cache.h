//
// Created by zhaojieyi on 2023/2/21.
//

// https://www.youtube.com/watch?v=lHnYSkZ7Cis
// Python-Inspired Function Cache for C++
#ifndef CODESNIPPET_FUNC_CACHE_H
#define CODESNIPPET_FUNC_CACHE_H

#include <tuple>
#include <type_traits>
#include <map>

// intentionally copies the params
template<typename Func, typename ...Params>
auto cache(Func func, Params && ...params) {
  using param_set = std::tuple<std::remove_cvref_t<Params>...>;

  param_set key{params...};

  using result_type = std::remove_cvref_t<std::invoke_result<Func, decltype(params)...>>;

  // this is not thread safe, it's basically a global, this is not good
  static std::map<param_set, result_type> cached_values;
  using value_type = typename decltype(cached_values)::value_type;

  auto itr = cached_values.find(key);
  if (itr!=cached_values.end()) {
    return itr->second;
  }

  return cached_values.insert(value_type{std::move(key), func(std::forward<Params>(params)...)}).first->second;
}

#endif // CODESNIPPET_FUNC_CACHE_H
