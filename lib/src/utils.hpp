#pragma once

// std libs:
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

using std::size_t;

/**
 * @brief Выводит все элементы пары в поток
 * @tparam Type1: тип, возможный к выводу в консоль
 * @tparam Type2: тип, возможный к выводу в консоль
 * @param os: ссылка на поток, в который надо вывести (мод.)
 * @param pair: пара элементов произвольного типа
 * @return std::ostream&: ссылка на поток, в который вывели
 */
template <typename Type1, typename Type2>
inline std::ostream& operator<<(std::ostream& os,
                                const std::pair<Type1, Type2>& pair) {
  return os << "{" << pair.first << "; " << pair.second << "}";
}

/**
 * @brief Выводит все элементы std::tuple в поток
 * @tparam I: текущий индекс, обрабатываемый в кортеже
 * @tparam Ts: типы элементов в кортеже
 * @param os: выходной поток, в который будут записаны элементы кортежа
 * @param t: кортеж, который нужно распечатать
 * @return std::ostream&: модифицированный выходной поток
 */
template <std::size_t I = 0, typename... Ts>
static std::ostream& PrintTuple(std::ostream& os, const std::tuple<Ts...>& t) {
  if constexpr (I < sizeof...(Ts)) {
    if (I != 0) os << "; ";

    os << std::get<I>(t);
    return PrintTuple<I + 1, Ts...>(os, t);
  } else
    return os;
}

/**
 * @brief Выводит все элементы std::tuple в поток
 * @tparam Ts: типы элементов в кортеже.
 * @param os: выходной поток, в который будет записан кортеж.
 * @param t: кортеж, который нужно распечатать.
 * @return std::ostream&: модифицированный выходной поток
 */
template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& t) {
  os << "{";
  PrintTuple(os, t);
  return os << "}";
}

/**
 * @brief Выводит все элементы вектора в поток
 * @tparam Type: тип, возможный к выводу в консоль
 * @param os: ссылка на поток, в который надо вывести (мод.)
 * @param vec: вектор элементов произвольного типа
 * @return std::ostream&: ссылка на поток, в который вывели
 */
template <typename Type>
inline std::ostream& operator<<(std::ostream& os,
                                const std::vector<Type>& vec) {
  os << "{";

  for (std::size_t i = 0; i < vec.size(); i++) {
    os << vec[i];
    if (i != vec.size() - 1) os << "; ";
  }

  return os << "}";
}

/**
 * @brief Выводит все элементы std::unordered_map в выходной поток
 * @tparam K: тип ключей в неупорядоченной карте
 * @tparam V: тип значений в неупорядоченной карте
 * @param os: выходной поток, в который будет записан словарь
 * @param map: словарь, который нужно распечатать
 * @return std::ostream&: модифицированный выходной поток
 */
template <typename K, typename V>
std::ostream& operator<<(std::ostream& os,
                         const std::unordered_map<K, V>& map) {
  os << "{";

  bool first = true;
  for (const auto& [key, value] : map) {
    if (!first) os << "; ";

    os << key << ": " << value;
    first = false;
  }

  return os << "}";
}

/**
 * @brief функция, которая обрезает незнач. нули float при преобр. в строку
 * @param number: число типа float
 * @return std::string: итоговое число, записанное в строку
 */
inline std::string ErasedZerosStr(float number) {
  std::string str = std::to_string(number);

  // удаляем незначащие нули
  str.erase(str.find_last_not_of('0') + 1, std::string::npos);

  // если последний символ - десятичная точка, удаляем
  if (str.back() == '.') str.pop_back();

  return str;
}

/**
 * @brief перегрузка, которая вводит все элементы вектора из потока
 * (работает исключительно с консолью, так как
 * (вывод о текущем состоянии происходит туда)
 * @tparam Type: тип, возможный к выводу в консоль
 * @param is: ссылка на поток, из которого надо ввести (мод.)
 * @param vec: вектор элементов типа Type (мод.)
 * @return std::istream&: ссылка на поток, из которого ввели
 */
template <typename Type>
inline std::istream& operator>>(std::istream& is, std::vector<Type>& vec) {
  // @brief размер вектора
  long size = 0;

  std::cout << "Enter array size: ";
  while (size <= 0) {
    is >> size;
    if (!is) {
      std::cerr << "Invalid size input." << std::endl;
      return is;
    }

    if (size <= 0) std::cout << "Invalid size input. Try again: ";
  }

  // @brief текущий элемент
  Type curr;

  vec.clear();  // (для перезаписи нужна отчистка)
  std::cout << "Enter array elements: ";
  for (std::size_t i = 0; i < std::size_t(size); i++) {
    is >> curr;
    if (!is) {
      std::cerr << "Invalid array input. The entry is incorrect." << std::endl;
      return is;
    }

    vec.push_back(curr);
  }

  return is;
}

/**
 * @brief Проверяет наличие элемента в векторе
 * @tparam T: тип элемента
 * @param vec: исходный вектор
 * @param value: искомое значение
 * @return true: элемент найден
 * @return false: элемент не найден
 */
template <typename T>
bool Contains(const std::vector<T>& vec, const T& value) {
  auto it = std::find(vec.begin(), vec.end(), value);
  return it != vec.end();
}
