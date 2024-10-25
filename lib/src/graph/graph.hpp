#pragma once

#include "../lib/utils.hpp"

template <typename T1>
concept AllowedVertType =
    std::is_same_v<T1, std::string> || std::is_same_v<T1, short> ||
    std::is_same_v<T1, int> || std::is_same_v<T1, size_t>;

template <typename T2>
concept AllowedWeightType =
    std::is_same_v<T2, long> || std::is_same_v<T2, double>;

/**
 * @brief Класс графа (может быть взвешенным и ориентированным)
 * @details Поддерживаемые типы:
 *                         вес: short, int, long, double
 *                         вершина: std::string, size_t
 * @tparam vert_t: тип вершин
 * @tparam weight_t: тип весов
 */
template <typename vert_t, typename weight_t>
class Graph {
 public:
  /// @brief Инициализирует новый экземпляр Graph
  Graph() : edges_(), verts_() {
    static_assert(AllowedVertType<vert_t>,
                  "This vertice type parameter is not allowed");
    static_assert(AllowedWeightType<weight_t>,
                  "This weight type parameter is not allowed");
  }

  /**
   * @brief Создает новый экземпляр Graph по ребрам,
   * представленными вектором std::pair (НЕВЗВЕШЕННЫЙ)
   * @param edges_pairs: ребра графа
   * @return Graph: новый экземпляр Graph
   */
  static Graph GraphNonWeighted(
      const std::vector<std::pair<vert_t, vert_t>>& edges_pairs);

  /**
   * @brief Создает новый экземпляр Graph по ребрам,
   * представленными вектором std::pair и weight_t (ВЗВЕШЕННЫЙ)
   * @param edges_pairs: ребра графа
   * @param weights: веса ребер
   * @return Graph: новый экземпляр Graph
   * @throw std::invalid_argument("GraphWeighted: the sizes of the edges and
   * weights vectors do not match.");
   */
  static Graph GraphWeighted(
      const std::vector<std::pair<vert_t, vert_t>>& edges_pairs,
      const std::vector<weight_t>& weights);

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными вектором std::tuple (ВЗВЕШЕННЫЙ)
   * @param edges_tuples: ребра графа
   * @return Graph: новый экземпляр Graph
   */
  static Graph GraphWeighted(
      const std::vector<std::tuple<vert_t, vert_t, weight_t>>& edges_tuples);

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными вектором std::string (НЕВЗВЕШЕННЫЙ)
   * @param edges_strs: ребра графа
   * @return Graph: новый экземпляр Graph
   */
  static Graph GraphFromStrs(const std::vector<std::string>& edges_strs);

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными словарем из std::string и weight_t (ВЗВЕШЕННЫЙ)
   * @param edges_dict: ребра графа
   * @return Graph: новый экземпляр Graph
   */
  static Graph GraphFromMap(
      const std::unordered_map<std::string, weight_t>& edges_dict);

  /**
   * @brief Создает новый экземпляр Graph по матрице смежности
   * @param adj_matrix: матрица смежности
   * @param is_weighted: взвешен ли граф
   * @return Graph: новый экземпляр Graph
   * @throw std::invalid_argument("GraphFromAdjMatrix: AdjacencyMatrix is not
   * squared.");
   * @throw std::invalid_argument("GraphFromAdjMatrix: AdjacencyMatrix is not
   * squared [row problem].");
   * @throw std::logic_error("GraphFromAdjMatrix: this method (constructor) is
   * deleted for std::string.");
   */
  static Graph GraphFromAdjMatrix(
      const std::vector<std::vector<weight_t>>& adj_matrix,
      bool is_weighted = false);

  /**
   * @brief Создает новый экземпляр Graph
   * по списку смежности (НЕВЗВЕШЕННЫЙ)
   * @param adj_list: список смежности
   * @return Graph: новый экземпляр Graph
   * @throw std::logic_error("GraphFromAdjList: this method (constructor) is
   * deleted for std::string.");
   */
  static Graph GraphFromAdjList(
      const std::vector<std::vector<vert_t>>& adj_list);

  /**
   * @brief Создает новый экземпляр Graph
   * по списку смежности с указанием вершины-ключа (НЕВЗВЕШЕННЫЙ)
   * @param adj_list_dict: список смежности с указанием вершины-ключа
   * @return Graph: новый экземпляр Graph
   */
  static Graph GraphFromAdjList(
      const std::unordered_map<vert_t, std::vector<vert_t>>& adj_list_dict);

  /// @brief Проверяет, взвешен ли граф
  bool IsWeighted() const;

  /// @return size_t: кол-во вершин
  size_t VertsAmount() const { return verts_.size(); }

  /// @return vert_t: кол-во ребер
  size_t EdgesAmount() const { return edges_.size(); }

  /// @return const std::vector<vert_t>&: вершины
  const std::vector<vert_t>& Verts() const { return verts_; }

  /// @return std::vector<std::tuple<vert_t, vert_t, weight_t>>: ребра
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
   * @return std::ostream&: выходной поток
   */
  std::ostream& PrintEdges(std::ostream& os = std::cout) const;

  /**
   * @brief Выводит в поток список смежности
   * @param os: входной поток
   * @return std::ostream&: выходной поток
   */
  std::ostream& PrintAdjList(std::ostream& os = std::cout) const;

  /**
   * @brief Делает граф ненаправленным (удаляет лишние ребра)
   * @param remove_duplicates: удалять ли дубликаты
   */
  void MakeUndirected(bool remove_duplicates = false);

  /// @brief Делает граф направленным (ничего)
  void MakeDirected() { is_direct = true; }

  /// @brief Проверяет, направлен ли граф
  bool IsDirected() const { return is_direct; }

  /// @brief Удаляет из графа ребрами с одинаковым вершинами
  void RemoveDuplicates();

  /**
   * @return std::vector<std::vector<vert_t>>: список смежности
   * @throw std::logic_error("GetAdjListWithoutKeys: this method is deleted for
   * std::string.");
   */
  std::vector<std::vector<vert_t>> GetAdjListWithoutKeys() const;

  /// @return std::unordered_map<vert_t, std::vector<vert_t>>: список смежности
  /// с указанием вершины-ключа
  std::unordered_map<vert_t, std::vector<vert_t>> GetAdjList() const;

  /**
   * @return std::vector<std::vector<vert_t>>: матрица смежности
   * @throw std::logic_error("GetAdjMatrix: this method is deleted for
   * std::string.");
   */
  std::vector<std::vector<weight_t>> GetAdjMatrix() const;

  /**
   * @brief Проверяет, содержится ли вершина в графе
   * @param vert: вершина
   * @return true: содержится
   * @return false: не содержится
   */
  bool ContainsVert(const vert_t& vert) const {
    return std::find(Verts().begin(), Verts().end(), vert) != Verts().end();
  }

  /**
   * @brief Проверяет, содержится ли ребро в графе (ВЗВЕШЕННЫЙ)
   * @param edge: ребро
   * @return true: содержится
   * @return false: не содержится
   * @throw std::logic_error("ContainsEdge: graph is not weighted.");
   * @throw std::logic_error("ContainsEdge: weight must be greater than zero.");
   */
  bool ContainsEdge(const std::tuple<vert_t, vert_t, weight_t>& edge) const;

  /**
   * @brief Проверяет, содержится ли ребро в графе (НЕВЗВЕШЕННЫЙ)
   * @param edge: ребро
   * @return true: содержится
   * @return false: не содержится
   */
  bool ContainsEdge(const std::pair<vert_t, vert_t>& edge) const;

  /**
   * @brief Находит вес ребра в взвешенном графе
   * @param edge: ребро
   * @return weight_t: вес
   * @throw std::logic_error("GetWeightOfEdge: graph is not weighted.");
   * @throw std::invalid_argument("GetWeightOfEdge: there is no such edge:");
   */
  weight_t GetWeightOfEdge(const std::pair<vert_t, vert_t>& edge) const;

  void AddVert(vert_t vert);

  /// @throw std::invalid_argument(std::string("AddEdge: ") + ex.what());
  void AddEdge(vert_t start_vert, vert_t end_vert, weight_t weight);

  /**
   * @throw std::logic_error("AddEdge: weighted graph must consist of weighted
   * edges.");
   */
  void AddEdge(vert_t start_vert, vert_t end_vert);

  /// @throw std::invalid_argument("RemoveVert: there is no such vert:");
  void RemoveVert(vert_t vert);

  /// @throw std::invalid_argument("RemoveEdge: there is no such edge:");
  void RemoveEdge(const std::pair<vert_t, vert_t>& edge_pair);

  /// @throw std::invalid_argument("RemoveEdge: there is no such edge:");
  void RemoveEdge(const std::tuple<vert_t, vert_t, weight_t>& edge_tuple);

 private:
  class Edge {
   public:
    Edge() = delete;

    Edge(vert_t start_vert, vert_t end_vert)
        : start_vert_{start_vert}, end_vert_{end_vert} {}

    /// @throw std::invalid_argument("Edge: weight must be greater than zero.");
    Edge(vert_t start_vert, vert_t end_vert, weight_t weight);

    Edge(std::pair<vert_t, vert_t> edge_pair);
    Edge(std::tuple<vert_t, vert_t, weight_t> edge_tuple);

    bool IsWeighted() const { return weight_ != 0; }

    vert_t StartVert() const { return start_vert_; }
    vert_t EndVert() const { return end_vert_; }

    /// @throw std::logic_error("Edge: " + Name() + " is not weighted.");
    weight_t Weight() const;

    // friend Graph;

    bool operator==(const Edge& rhs) const {
      return start_vert_ == rhs.start_vert_ && end_vert_ == rhs.end_vert_ &&
             weight_ == rhs.weight_;
    }

    bool operator!=(const Edge& rhs) const { return !(*this == rhs); }

    /// @throw std::invalid_argument("Edge: unweighted edges are not
    /// comparable.");
    auto operator<=>(const Edge& rhs) const;

    const std::string& Name() const;

   private:
    vert_t start_vert_;
    vert_t end_vert_;
    weight_t weight_ = 0;
  };

  std::vector<Edge> edges_;
  std::vector<vert_t> verts_;

  bool is_direct = true;

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

template <typename vert_t, typename weight_t>
inline std::ostream& operator<<(std::ostream& os,
                                const Graph<vert_t, weight_t>& graph) {
  os << "Edges:\n     ";
  graph.PrintEdges(os);

  os << "\n";

  os << "Vertices:\n     ";
  graph.PrintVerts(os);
  return os;
}

template <typename vert_t, typename weight_t>
inline vert_t StartVertFromTuple(
    const std::tuple<vert_t, vert_t, weight_t>& edge) {
  return std::get<0>(edge);
}

template <typename vert_t, typename weight_t>
inline vert_t EndVertFromTuple(
    const std::tuple<vert_t, vert_t, weight_t>& edge) {
  return std::get<1>(edge);
}

template <typename vert_t, typename weight_t>
inline weight_t WeightFromTuple(
    const std::tuple<vert_t, vert_t, weight_t>& edge) {
  return std::get<2>(edge);
}
