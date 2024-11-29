#pragma once

#include "../lib/utils.hpp"

template <typename vert_t>
concept AllowedVertType =
    std::is_same_v<vert_t, std::string> || std::is_same_v<vert_t, char> ||
    std::is_same_v<vert_t, short> || std::is_same_v<vert_t, int> ||
    std::is_same_v<vert_t, long> || std::is_same_v<vert_t, size_t>;

template <typename weight_t>
concept AllowedWeightType =
    std::is_same_v<weight_t, char> || std::is_same_v<weight_t, short> ||
    std::is_same_v<weight_t, int> || std::is_same_v<weight_t, long> ||
    std::is_same_v<weight_t, size_t> || std::is_same_v<weight_t, float> ||
    std::is_same_v<weight_t, double> || std::is_same_v<weight_t, long double>;

template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline vert_t StartVertFromTuple(
    const std::tuple<vert_t, vert_t, weight_t>& edge) {
  return std::get<0>(edge);
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline vert_t EndVertFromTuple(
    const std::tuple<vert_t, vert_t, weight_t>& edge) {
  return std::get<1>(edge);
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline weight_t WeightFromTuple(
    const std::tuple<vert_t, vert_t, weight_t>& edge) {
  return std::get<2>(edge);
}

/**
 * @brief Класс графа (может быть взвешенным и ориентированным)
 * @details Поддерживаемые типы:
 * вес: char, short, int, long, size_t, float, double, long double
 * вершины: std::string, char, short, int, long, size_t
 * @tparam vert_t: тип вершин
 * @tparam weight_t: тип весов
 */
template <AllowedVertType vert_t = std::string,
          AllowedWeightType weight_t = size_t>
class Graph {
 public:
  /// @brief Инициализирует новый экземпляр Graph
  Graph() : edges_(), verts_() {}

  /**
   * @brief Копирующий конструктор
   * @param other: граф, который нужно скопировать
   */
  Graph(const Graph& other) = default;

  /**
   * @brief Оператор копирующего присваивания
   * @param other: граф, значения которого нужно присвоить
   * @return `Graph&`: ссылка на текущий объект
   */
  Graph& operator=(const Graph& other) = default;

  /**
   * @brief Перемещающий конструктор. Перемещает ресурсы из другого графа
   * @param other: граф, ресурсы которого нужно переместить
   */
  Graph(Graph&& other) noexcept
      : edges_(std::move(other.edges_)),
        verts_(std::move(other.verts_)),
        is_direct_{other.is_direct_} {}

  /**
   * @brief Оператор перемещающего присваивания. Перемещает ресурсы из другого
   * графа.
   * @param other: граф, ресурсы которого нужно переместить
   * @return `Graph&`: ссылка на текущий объект
   */
  Graph& operator=(Graph&& other) noexcept {
    if (this != &other) {
      edges_ = std::move(other.edges_);
      verts_ = std::move(other.verts_);
      is_direct_ = other.is_direct_;
    }

    return *this;
  }

  /**
   * @brief Создает новый экземпляр Graph по ребрам,
   * представленными вектором std::pair (НЕВЗВЕШЕННЫЙ)
   * @param edges_pairs: ребра графа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphNonWeighted(
      const std::vector<std::pair<vert_t, vert_t>>& edges_pairs);

  /**
   * @brief Создает новый экземпляр Graph по ребрам,
   * представленными вектором std::pair и weight_t (ВЗВЕШЕННЫЙ)
   * @param edges_pairs: ребра графа
   * @param weights: веса ребер
   * @return `Graph`: новый экземпляр Graph
   * @throw `std::invalid_argument("GraphWeighted: the sizes of the edges and
   * weights vectors do not match.")`
   */
  static Graph GraphWeighted(
      const std::vector<std::pair<vert_t, vert_t>>& edges_pairs,
      const std::vector<weight_t>& weights);

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными вектором std::tuple (ВЗВЕШЕННЫЙ)
   * @param edges_tuples: ребра графа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphWeighted(
      const std::vector<std::tuple<vert_t, vert_t, weight_t>>& edges_tuples);

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными вектором std::string (НЕВЗВЕШЕННЫЙ)
   * @param edges_strs: ребра графа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphFromStrs(const std::vector<std::string>& edges_strs);

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными словарем из std::string и weight_t (ВЗВЕШЕННЫЙ)
   * @param edges_dict: ребра графа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphFromMap(
      const std::unordered_map<std::string, weight_t>& edges_dict);

  /**
   * @brief Создает новый экземпляр Graph по матрице смежности
   * @param adj_matrix: матрица смежности
   * @param is_weighted: взвешен ли граф
   * @return `Graph`: новый экземпляр Graph
   * @throw `std::invalid_argument("GraphFromAdjMatrix: AdjacencyMatrix is not
   * squared.")`
   * @throw `std::invalid_argument("GraphFromAdjMatrix: AdjacencyMatrix is not
   * squared [row problem].")`
   * @throw `std::logic_error("GraphFromAdjMatrix: this method (constructor) is
   * deleted for std::string.")`
   */
  static Graph GraphFromAdjMatrix(
      const std::vector<std::vector<weight_t>>& adj_matrix,
      bool is_weighted = false);

  /**
   * @brief Создает новый экземпляр Graph
   * по списку смежности (НЕВЗВЕШЕННЫЙ)
   * @param adj_list: список смежности
   * @return `Graph`: новый экземпляр Graph
   * @throw std::logic_error("GraphFromAdjList: this method (constructor) is
   * deleted for std::string.");
   */
  static Graph GraphFromAdjList(
      const std::vector<std::vector<vert_t>>& adj_list);

  /**
   * @brief Создает новый экземпляр Graph
   * по списку смежности с указанием вершины-ключа (НЕВЗВЕШЕННЫЙ)
   * @param adj_list_dict: список смежности с указанием вершины-ключа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphFromAdjList(
      const std::unordered_map<vert_t, std::vector<vert_t>>& adj_list_dict);

  /// @brief Проверяет, взвешен ли граф
  bool IsWeighted() const;

  /// @return `size_t`: кол-во вершин
  size_t VertsAmount() const { return verts_.size(); }

  /// @return `vert_t`: кол-во ребер
  size_t EdgesAmount() const { return edges_.size(); }

  /// @return `const std::vector<vert_t>&`: вершины
  const std::vector<vert_t>& Verts() const { return verts_; }

  /// @return `std::vector<std::tuple<vert_t, vert_t, weight_t>>`: ребра
  std::vector<std::tuple<vert_t, vert_t, weight_t>> Edges() const;

  /**
   * @brief Выводит в поток список вершин
   * @param os: входной поток
   * @return std::ostream&: выходной поток
   */
  std::ostream& PrintVerts(std::ostream& os = std::cout) const;

  /**
   * @brief Выводит в поток список ребер
   * @param os: входной поток
   * @return `std::ostream&`: выходной поток
   */
  std::ostream& PrintEdges(std::ostream& os = std::cout) const;

  /**
   * @brief Выводит в поток список смежности
   * @param os: входной поток
   * @return `std::ostream&`: выходной поток
   */
  std::ostream& PrintAdjList(std::ostream& os = std::cout) const;

  /**
   * @brief Делает граф ненаправленным (удаляет лишние ребра)
   * @param remove_duplicates: удалять ли дубликаты
   */
  void MakeUndirected(bool remove_duplicates = false);

  /// @brief Делает граф направленным (ничего)
  void MakeDirected() { is_direct_ = true; }

  /// @brief Проверяет, направлен ли граф
  bool IsDirected() const { return is_direct_; }

  /// @brief Удаляет из графа ребрами с одинаковым вершинами
  void RemoveDuplicates();

  /**
   * @return `std::vector<std::vector<vert_t>>`: список смежности
   * @throw `std::logic_error("GetAdjListWithoutKeys: this method is deleted for
   * std::string.")`
   */
  std::vector<std::vector<vert_t>> GetAdjListWithoutKeys() const;

  /// @return `std::unordered_map<vert_t, std::vector<vert_t>>`: список
  /// смежности с указанием вершины-ключа
  std::unordered_map<vert_t, std::vector<vert_t>> GetAdjList() const;

  /**
   * @return `std::vector<std::vector<vert_t>>`: матрица смежности
   * @throw `std::logic_error("GetAdjMatrix: this method is deleted for
   * std::string.")`
   */
  std::vector<std::vector<weight_t>> GetAdjMatrix() const;

  /**
   * @brief Проверяет, содержится ли вершина в графе
   * @param vert: вершина
   * @return `true`: содержится
   * @return `false`: не содержится
   */
  bool ContainsVert(const vert_t& vert) const {
    return std::find(Verts().begin(), Verts().end(), vert) != Verts().end();
  }

  /**
   * @brief Проверяет, содержится ли ребро в графе (ВЗВЕШЕННЫЙ)
   * @param edge: ребро
   * @return `true`: содержится
   * @return `false`: не содержится
   * @throw `std::logic_error("ContainsEdge: graph is not weighted.")`
   * @throw `std::logic_error("ContainsEdge: weight must be greater than
   * zero.")`
   */
  bool ContainsEdge(const std::tuple<vert_t, vert_t, weight_t>& edge) const;

  /**
   * @brief Проверяет, содержится ли ребро в графе (НЕВЗВЕШЕННЫЙ)
   * @param edge: ребро
   * @return `true`: содержится
   * @return `false`: не содержится
   */
  bool ContainsEdge(const std::pair<vert_t, vert_t>& edge) const;

  /**
   * @brief Находит вес ребра в взвешенном графе
   * @param edge: ребро
   * @return `weight_t`: вес
   * @throw `std::logic_error("GetWeightOfEdge: graph is not weighted.")`
   * @throw `std::invalid_argument("GetWeightOfEdge: there is no such edge:")`
   */
  weight_t GetWeightOfEdge(const std::pair<vert_t, vert_t>& edge) const;

  void AddVert(vert_t vert);

  /// @throw `std::invalid_argument(std::string("AddEdge: ") + ex.what())`
  void AddEdge(vert_t start_vert, vert_t end_vert, weight_t weight);

  /**
   * @throw `std::logic_error("AddEdge: weighted graph must consist of weighted
   * edges.")`
   */
  void AddEdge(vert_t start_vert, vert_t end_vert);

  /// @throw `std::invalid_argument("RemoveVert: there is no such vert:")`
  void RemoveVert(vert_t vert);

  /// @throw `std::invalid_argument("RemoveEdge: there is no such edge:")`
  void RemoveEdge(const std::pair<vert_t, vert_t>& edge_pair);

  /// @throw `std::invalid_argument("RemoveEdge: there is no such edge:")`
  void RemoveEdge(const std::tuple<vert_t, vert_t, weight_t>& edge_tuple);

 private:
  class Edge {
   public:
    Edge() = delete;

    Edge(vert_t start_vert, vert_t end_vert)
        : start_vert_{start_vert}, end_vert_{end_vert} {}

    Edge(vert_t start_vert, vert_t end_vert, weight_t weight)
        : start_vert_{start_vert}, end_vert_{end_vert}, weight_{weight} {}

    Edge(std::pair<vert_t, vert_t> edge_pair)
        : start_vert_{edge_pair.first}, end_vert_{edge_pair.second} {}

    Edge(std::tuple<vert_t, vert_t, weight_t> edge_tuple)
        : start_vert_{StartVertFromTuple(edge_tuple)},
          end_vert_{EndVertFromTuple(edge_tuple)},
          weight_{WeightFromTuple(edge_tuple)} {}

    bool IsWeighted() const { return weight_ != 0; }

    vert_t StartVert() const { return start_vert_; }
    vert_t EndVert() const { return end_vert_; }

    weight_t Weight() const { return weight_; }

    // friend Graph;

    bool operator==(const Edge& rhs) const {
      return start_vert_ == rhs.start_vert_ && end_vert_ == rhs.end_vert_ &&
             weight_ == rhs.weight_;
    }

    bool operator!=(const Edge& rhs) const { return !(*this == rhs); }

    auto operator<=>(const Edge& rhs) const { return weight_ <=> rhs.Weight(); }

    const std::string& Name() const;

   private:
    vert_t start_vert_;
    vert_t end_vert_;
    weight_t weight_ = 0;
  };

  std::vector<Edge> edges_;
  std::vector<vert_t> verts_;

  bool is_direct_ = true;

 public:
  friend std::ostream& operator<<(std::ostream& os,
                                  const Graph<vert_t, weight_t>::Edge& edge) {
    os << edge.Name();
    return os;
  }

 private:
  Graph(const std::vector<Edge>& edges);

  static std::pair<vert_t, vert_t> ParseEdgeString(const std::string& edge_str);
};

template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline std::ostream& operator<<(std::ostream& os,
                                const Graph<vert_t, weight_t>& graph) {
  os << "Edges:\n     ";
  graph.PrintEdges(os);

  os << "\n";

  os << "Vertices:\n     ";
  graph.PrintVerts(os);
  return os;
}

#define GRAPH_TEMPLATE_CONSTRUCT_FOR_STRING  \
  template class Graph<std::string, short>;  \
  template class Graph<std::string, int>;    \
  template class Graph<std::string, long>;   \
  template class Graph<std::string, size_t>; \
  template class Graph<std::string, float>;  \
  template class Graph<std::string, double>; \
  template class Graph<std::string, long double>

#define GRAPH_TEMPLATE_CONSTRUCT_FOR_INTEGRAL \
  template class Graph<char, char>;           \
  template class Graph<char, short>;          \
  template class Graph<char, int>;            \
  template class Graph<char, long>;           \
  template class Graph<char, size_t>;         \
  template class Graph<char, float>;          \
  template class Graph<char, double>;         \
  template class Graph<char, long double>;    \
                                              \
  template class Graph<short, char>;          \
  template class Graph<short, short>;         \
  template class Graph<short, int>;           \
  template class Graph<short, long>;          \
  template class Graph<short, size_t>;        \
  template class Graph<short, float>;         \
  template class Graph<short, double>;        \
  template class Graph<short, long double>;   \
                                              \
  template class Graph<int, char>;            \
  template class Graph<int, short>;           \
  template class Graph<int, int>;             \
  template class Graph<int, long>;            \
  template class Graph<int, size_t>;          \
  template class Graph<int, float>;           \
  template class Graph<int, double>;          \
  template class Graph<int, long double>;     \
                                              \
  template class Graph<long, char>;           \
  template class Graph<long, short>;          \
  template class Graph<long, int>;            \
  template class Graph<long, long>;           \
  template class Graph<long, size_t>;         \
  template class Graph<long, float>;          \
  template class Graph<long, double>;         \
  template class Graph<long, long double>;    \
                                              \
  template class Graph<size_t, char>;         \
  template class Graph<size_t, short>;        \
  template class Graph<size_t, int>;          \
  template class Graph<size_t, long>;         \
  template class Graph<size_t, size_t>;       \
  template class Graph<size_t, float>;        \
  template class Graph<size_t, double>;       \
  template class Graph<size_t, long double>
