#ifndef ENUMHASHMAP_H
#define ENUMHASHMAP_H

#include <unordered_map>

struct EnumClassHash {
  template <typename T>
  std::size_t operator()(T t) const {
    return static_cast<std::size_t>(t);
  }
};

template <typename K, typename V>
using EnumHashMap = std::unordered_map<K, V, EnumClassHash>;

#endif  // ENUMHASHMAP_H
