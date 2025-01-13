#pragma once

#include "utils.hpp"

// MARK: Concepts

template <typename vert_t>
concept AllowedVertType =
    std::is_arithmetic_v<vert_t> || std::is_same_v<vert_t, std::string>;

template <typename weight_t>
concept AllowedWeightType = std::is_arithmetic_v<weight_t>;

// MARK: VertFromTuple

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

// MARK: Graph

/**
 * @brief Класс графа (может быть взвешенным и ориентированным)
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
      const std::vector<std::pair<vert_t, vert_t>>& edges_pairs) {
    if (edges_pairs.empty()) return Graph();

    std::vector<Edge> edges{};
    edges.reserve(edges_pairs.size());

    for (const auto& edge : edges_pairs) edges.push_back(edge);

    return Graph(edges);
  }

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
      const std::vector<weight_t>& weights) {
    if (edges_pairs.empty() && weights.empty()) return Graph();

    std::vector<Edge> edges{};
    edges.reserve(edges_pairs.size());

    if (edges_pairs.size() != weights.size())
      throw std::invalid_argument(
          "GraphWeighted: the sizes of the edges and weights vectors do not "
          "match.");

    for (size_t i = 0; i < weights.size(); i++)
      edges.push_back(Edge(edges_pairs[i], weights[i]));

    return Graph(edges);
  }

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными вектором std::tuple (ВЗВЕШЕННЫЙ)
   * @param edges_tuples: ребра графа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphWeighted(
      const std::vector<std::tuple<vert_t, vert_t, weight_t>>& edges_tuples) {
    if (edges_tuples.empty()) return Graph();

    std::vector<Edge> edges;

    for (const auto& [start, end, weight] : edges_tuples)
      edges.emplace_back(start, end, weight);

    return Graph(edges);
  }

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными вектором std::string (НЕВЗВЕШЕННЫЙ)
   * @param edges_strs: ребра графа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphFromStrs(const std::vector<std::string>& edges_strs) {
    if (edges_strs.empty()) return Graph();

    std::vector<Graph<vert_t, weight_t>::Edge> edges;

    for (const auto& edge_str : edges_strs) {
      vert_t start_vert, end_vert;
      std::tie(start_vert, end_vert) = ParseEdgeString_(edge_str);

      edges.emplace_back(start_vert, end_vert);
    }

    return Graph(edges);
  }

  /**
   * @brief Создает новый экземпляр Graph по ребрам
   * представленными словарем из std::string и weight_t (ВЗВЕШЕННЫЙ)
   * @param edges_dict: ребра графа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphFromMap(
      const std::unordered_map<std::string, weight_t>& edges_dict) {
    if (edges_dict.empty()) return Graph();

    std::vector<Graph<vert_t, weight_t>::Edge> edges;

    for (const auto& [edge_str, weight] : edges_dict) {
      vert_t start_vert, end_vert;
      std::tie(start_vert, end_vert) = ParseEdgeString_(edge_str);

      edges.emplace_back(start_vert, end_vert, weight);
    }

    return Graph(edges);
  }

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
      bool is_weighted = false) {
    if constexpr (std::is_arithmetic_v<vert_t>) {
      if (adj_matrix.empty()) return Graph();

      std::vector<Edge> edges{};

      if (adj_matrix.size() != adj_matrix[0].size())
        throw std::invalid_argument(
            "GraphFromAdjMatrix: AdjacencyMatrix is not squared.");

      for (size_t row = 0; row < adj_matrix.size(); row++) {
        if (row != 0)
          if (adj_matrix[row].size() != adj_matrix[row - 1].size())
            throw std::invalid_argument(
                "GraphFromAdjMatrix: AdjacencyMatrix is not squared [row "
                "problem].");

        for (size_t col = 0; col < adj_matrix[row].size(); col++) {
          if (adj_matrix[row][col] == 0) continue;

          if (is_weighted)
            edges.push_back(Edge(col, row, adj_matrix[col][row]));
          else
            edges.push_back(Edge(col, row));
        }
      }

      return Graph(edges);

    } else if constexpr (std::is_same_v<vert_t, std::string>)
      throw std::logic_error(
          "GraphFromAdjMatrix: this method (constructor) "
          "is deleted for std::string.");
  }

  /**
   * @brief Создает новый экземпляр Graph
   * по списку смежности (НЕВЗВЕШЕННЫЙ)
   * @param adj_list: список смежности
   * @return `Graph`: новый экземпляр Graph
   * @throw std::logic_error("GraphFromAdjList: this method (constructor) is
   * deleted for std::string.");
   */
  static Graph GraphFromAdjList(
      const std::vector<std::vector<vert_t>>& adj_list) {
    if constexpr (std::is_arithmetic_v<vert_t>) {
      if (adj_list.empty()) return Graph();

      std::vector<Edge> edges{};

      for (size_t row = 0; row < adj_list.size(); row++)
        for (size_t col = 0; col < adj_list[row].size(); col++)
          edges.push_back(Edge(row, adj_list[row][col]));

      return Graph(edges);

    } else if constexpr (std::is_same_v<vert_t, std::string>)
      throw std::logic_error(
          "GraphFromAdjList: this method (constructor) "
          "is deleted for std::string.");
  }

  /**
   * @brief Создает новый экземпляр Graph
   * по списку смежности с указанием вершины-ключа (НЕВЗВЕШЕННЫЙ)
   * @param adj_list_dict: список смежности с указанием вершины-ключа
   * @return `Graph`: новый экземпляр Graph
   */
  static Graph GraphFromAdjList(
      const std::unordered_map<vert_t, std::vector<vert_t>>& adj_list_dict) {
    if (adj_list_dict.empty()) return Graph();

    std::vector<Edge> edges{};

    for (const auto& vert_str_pair : adj_list_dict) {
      auto vert = vert_str_pair.first;
      for (const auto& vert_2 : vert_str_pair.second)
        edges.push_back(Edge(vert, vert_2));
    }

    return Graph(edges);
  }

  /// @brief Проверяет, взвешен ли граф
  bool IsWeighted() const { return is_weighted_; }

  /// @return `size_t`: кол-во вершин
  size_t VertsAmount() const { return verts_.size(); }

  /// @return `vert_t`: кол-во ребер
  size_t EdgesAmount() const { return edges_.size(); }

  /// @return `const std::vector<vert_t>&`: вершины
  const std::vector<vert_t>& Verts() const { return verts_; }

  /// @return `std::vector<std::tuple<vert_t, vert_t, weight_t>>`: ребра
  std::vector<std::tuple<vert_t, vert_t, weight_t>> Edges() const {
    if (edges_.empty()) return {};

    std::vector<std::tuple<vert_t, vert_t, weight_t>> edges_tuples(
        edges_.size());
    std::transform(edges_.begin(), edges_.end(), edges_tuples.begin(),
                   [](const Edge& edge) {
                     return std::make_tuple(edge.StartVert(), edge.EndVert(),
                                            edge.Weight());
                   });

    return edges_tuples;
  }

  /**
   * @brief Выводит в поток список вершин
   * @param os: входной поток
   * @return std::ostream&: выходной поток
   */
  std::ostream& PrintVerts(std::ostream& os = std::cout) const {
    os << Verts();
    return os;
  }

  /**
   * @brief Выводит в поток список ребер
   * @param os: входной поток
   * @return `std::ostream&`: выходной поток
   */
  std::ostream& PrintEdges(std::ostream& os = std::cout) const {
    os << edges_;
    return os;
  }

  /**
   * @brief Выводит в поток список смежности
   * @param os: входной поток
   * @return `std::ostream&`: выходной поток
   */
  std::ostream& PrintAdjList(std::ostream& os = std::cout) const {
    for (const auto& vert : Verts()) {
      os << vert << ": ";

      for (const auto& neighbor : edges_) {
        if (neighbor.StartVert() == vert) os << neighbor.EndVert() << "; ";
        if (!IsDirected())
          if (neighbor.EndVert() == vert) os << neighbor.StartVert() << "; ";
      }

      os << "\n";
    }

    return os;
  }

  /// @brief Делает граф ненаправленным (удаляет лишние ребра)
  void MakeUndirected() {
    std::unordered_set<size_t> seen_edges;
    std::vector<Edge> unique_edges;
    unique_edges.reserve(EdgesAmount());

    for (size_t i = 0; i < EdgesAmount(); i++) {
      if (seen_edges.count(i) != 0) continue;

      for (size_t j = i + 1; j < EdgesAmount(); j++)
        if (edges_[i].StartVert() == edges_[j].EndVert() &&
            edges_[j].StartVert() == edges_[i].EndVert()) {
          seen_edges.insert(j);
          break;
        }

      unique_edges.push_back(edges_[i]);
    }

    edges_ = std::move(unique_edges);
    is_direct_ = false;
  }

  /// @brief Делает граф направленным (ничего)
  void MakeDirected() { is_direct_ = true; }

  /// @brief Проверяет, направлен ли граф
  bool IsDirected() const { return is_direct_; }

  /**
   * @return `std::vector<std::vector<vert_t>>`: список смежности
   * @throw `std::logic_error("GetAdjListWithoutKeys: this method is deleted
   * for std::string.")`
   */
  std::vector<std::vector<vert_t>> GetAdjListWithoutKeys() const {
    if constexpr (std::is_arithmetic_v<vert_t>) {
      std::vector<std::vector<vert_t>> adj_list(
          *std::max_element(Verts().begin(), Verts().end()) + 1);

      for (const auto& edge : edges_) {
        adj_list[edge.StartVert()].push_back(edge.EndVert());
        if (!IsDirected()) adj_list[edge.EndVert()].push_back(edge.StartVert());
      }

      return adj_list;
    }

    else if constexpr (std::is_same_v<vert_t, std::string>)
      throw std::logic_error(
          "GetAdjListWithoutKeys: this method is deleted for std::string.");
  }

  /// @return `std::unordered_map<vert_t, std::vector<vert_t>>`: список
  /// смежности с указанием вершины-ключа
  std::unordered_map<vert_t, std::vector<vert_t>> GetAdjList() const {
    auto adj_list_dict = std::unordered_map<vert_t, std::vector<vert_t>>();

    for (const auto& edge : edges_) {
      adj_list_dict[edge.StartVert()].push_back(edge.EndVert());
      if (!IsDirected())
        adj_list_dict[edge.EndVert()].push_back(edge.StartVert());
    }

    return adj_list_dict;
  }

  /**
   * @return `std::vector<std::vector<vert_t>>`: матрица смежности
   * @throw `std::logic_error("GetAdjMatrix: this method is deleted for
   * std::string.")`
   */
  std::vector<std::vector<weight_t>> GetAdjMatrix() const {
    if constexpr (std::is_arithmetic_v<vert_t>) {
      std::vector<std::vector<weight_t>> adj_matrix(
          VertsAmount(), std::vector<weight_t>(VertsAmount(), 0));

      for (const auto& edge : edges_)
        if (edge.IsWeighted()) {
          adj_matrix[edge.StartVert()][edge.EndVert()] = edge.Weight();
          if (!IsDirected())
            adj_matrix[edge.EndVert()][edge.StartVert()] = edge.Weight();
        } else {
          adj_matrix[edge.StartVert()][edge.EndVert()] = 1;
          if (!IsDirected()) adj_matrix[edge.EndVert()][edge.StartVert()] = 1;
        }

      return adj_matrix;
    }

    else if constexpr (std::is_same_v<vert_t, std::string>)
      throw std::logic_error(
          "GetAdjMatrix: this method is deleted for std::string.");
  }

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
   */
  bool ContainsEdge(const std::tuple<vert_t, vert_t, weight_t>& edge) const {
    if (!IsWeighted())
      throw std::logic_error("ContainsEdge: graph is not weighted.");

    return GetEdgeIter_(edge) != edges_.end();
  }

  /**
   * @brief Проверяет, содержится ли ребро в графе (НЕВЗВЕШЕННЫЙ)
   * @param edge: ребро
   * @return `true`: содержится
   * @return `false`: не содержится
   */
  bool ContainsEdge(const std::pair<vert_t, vert_t>& edge) const {
    return GetEdgeIter_(edge) != edges_.end();
  }

  /**
   * @brief Находит вес ребра в взвешенном графе
   * @param edge: ребро
   * @return `weight_t`: вес
   * @throw `std::logic_error("GetEdgeWeight: graph is not weighted.")`
   * @throw `std::invalid_argument("GetEdgeWeight: there is no such edge:")`
   */
  weight_t GetEdgeWeight(const std::pair<vert_t, vert_t>& edge) const {
    if (!IsWeighted())
      throw std::logic_error("GetEdgeWeight: graph is not weighted.");

    auto it = GetEdgeIter_(edge);

    if (it == edges_.end())
      throw std::invalid_argument("GetEdgeWeight: there is no such edge: " +
                                  Edge(edge).Name());

    return it->Weight();
  }

  /**
   * @brief Меняет вес ребра в взвешенном графе
   * @param edge: ребро
   * @param new_weight: вес
   * @throw `std::logic_error("SetEdgeWeight: graph is not weighted.")`
   * @throw `std::invalid_argument("SetEdgeWeight: there is no such edge:")`
   */
  void SetEdgeWeight(const std::pair<vert_t, vert_t>& edge,
                     weight_t new_weight) {
    if (!IsWeighted())
      throw std::logic_error("SetEdgeWeight: graph is not weighted.");

    auto it = GetEdgeIter_(edge);

    if (it == edges_.end())
      throw std::invalid_argument("SetEdgeWeight: there is no such edge: " +
                                  Edge(edge).Name());

    it->SetWeight(new_weight);
  }

  void AddVert(const vert_t& vert) {
    if (!Contains(verts_, vert)) verts_.push_back(vert);
  }

  /// @warning `"AddEdge: weighted graph must consist of weighted edges.`
  void AddEdge(const std::tuple<vert_t, vert_t, weight_t>& edge_tuple,
               bool ignore_warning = false) {
    if (WeightFromTuple(edge_tuple) == 0)
      AddEdge({StartVertFromTuple(edge_tuple), EndVertFromTuple(edge_tuple)},
              ignore_warning);
    else
      AddEdge_(edge_tuple);
  }

  /// @warning `"AddEdge: weighted graph must consist of weighted edges.`
  void AddEdge(const std::pair<vert_t, vert_t>& edge_pair,
               bool ignore_warning = false) {
    if (IsWeighted() && !ignore_warning)
      std::cerr << "Warning! AddEdge: weighted graph should consist of "
                   "weighted edges."
                << std::endl;

    AddEdge_(Edge(edge_pair.first, edge_pair.second, static_cast<weight_t>(0)));
  }

  /// @throw `std::invalid_argument("RemoveVert: there is no such vert:")`
  void RemoveVert(const vert_t& vert) {
    if constexpr (std::is_arithmetic_v<vert_t>) {
      if (!Contains(Verts(), vert))
        throw std::invalid_argument(
            "RemoveVert: there is no such vert in graph: " +
            std::to_string(vert));
    }

    else if constexpr (std::is_same_v<vert_t, std::string>)
      if (!Contains(Verts(), vert))
        throw std::invalid_argument(
            "RemoveVert: there is no such vert in graph: " + vert);

    verts_.erase(std::remove(verts_.begin(), verts_.end(), vert), verts_.end());

    edges_.erase(std::remove_if(edges_.begin(), edges_.end(),
                                [vert](const Edge& edge) {
                                  return edge.StartVert() == vert ||
                                         edge.EndVert() == vert;
                                }),
                 edges_.end());
  }

  /// @throw `std::invalid_argument("RemoveEdge: there is no such edge:")`
  void RemoveEdge(const std::pair<vert_t, vert_t>& edge_pair) {
    if (!ContainsEdge(edge_pair))
      throw std::invalid_argument(
          "RemoveEdge: there is no such edge in graph: " +
          Edge(edge_pair).Name());

    edges_.erase(std::remove_if(edges_.begin(), edges_.end(),
                                [&edge_pair, this](const Edge& e) {
                                  return (e == Edge(edge_pair)) ||
                                         (!IsDirected() &&
                                          Edge(e.EndVert(), e.StartVert()) ==
                                              Edge(edge_pair));
                                }),
                 edges_.end());
  }

  /// @throw `std::invalid_argument("RemoveEdge: there is no such edge:")`
  void RemoveEdge(const std::tuple<vert_t, vert_t, weight_t>& edge_tuple) {
    if (!ContainsEdge(edge_tuple))
      throw std::invalid_argument(
          "RemoveEdge: there is no such edge in graph: " +
          Edge(edge_tuple).Name());

    edges_.erase(
        std::remove_if(
            edges_.begin(), edges_.end(),
            [&edge_tuple, this](const Edge& e) {
              return (e == Edge(edge_tuple)) ||
                     (!IsDirected() &&
                      e == Edge(std::make_tuple(EndVertFromTuple(edge_tuple),
                                                StartVertFromTuple(edge_tuple),
                                                WeightFromTuple(edge_tuple))));
            }),
        edges_.end());
  }

 private:
  class Edge {
   public:
    Edge() = delete;

    Edge(const vert_t start_vert, const vert_t& end_vert)
        : start_vert_{start_vert}, end_vert_{end_vert} {}

    Edge(const vert_t& start_vert, vert_t end_vert, weight_t weight)
        : start_vert_{start_vert}, end_vert_{end_vert}, weight_{weight} {}

    Edge(const std::pair<vert_t, vert_t>& edge_pair)
        : start_vert_{edge_pair.first}, end_vert_{edge_pair.second} {}

    Edge(const std::pair<vert_t, vert_t>& edge_pair, weight_t weight)
        : start_vert_{edge_pair.first},
          end_vert_{edge_pair.second},
          weight_{weight} {}

    Edge(const std::tuple<vert_t, vert_t, weight_t>& edge_tuple)
        : start_vert_{StartVertFromTuple(edge_tuple)},
          end_vert_{EndVertFromTuple(edge_tuple)},
          weight_{WeightFromTuple(edge_tuple)} {}

    bool IsWeighted() const { return weight_ != 0; }

    vert_t StartVert() const { return start_vert_; }
    vert_t EndVert() const { return end_vert_; }

    weight_t Weight() const { return weight_; }

    void SetWeight(weight_t new_weight) { weight_ = new_weight; }

    bool operator==(const Edge& rhs) const {
      if (StartVert() != rhs.StartVert() || EndVert() != rhs.EndVert())
        return false;

      if (IsWeighted() && rhs.IsWeighted()) return Weight() == rhs.Weight();

      return true;
    }

    bool operator!=(const Edge& rhs) const { return !(*this == rhs); }

    auto operator<=>(const Edge& rhs) const { return weight_ <=> rhs.Weight(); }

    const std::string& Name() const {
      static std::string name;

      if constexpr (std::is_arithmetic_v<vert_t>) {
        if (IsWeighted())
          name = "[" + std::to_string(StartVert()) + "->" +
                 std::to_string(EndVert()) +
                 ", w: " + std::to_string(Weight()) + "]";
        else
          name = "[" + std::to_string(StartVert()) + "->" +
                 std::to_string(EndVert()) + "]";

        // example: "[4->5]"

      } else if constexpr (std::is_same_v<vert_t, std::string>) {
        if (IsWeighted())
          name = "['" + StartVert() + "'->'" + EndVert() +
                 "', w: " + std::to_string(Weight()) + "]";
        else
          name = "['" + StartVert() + "'->'" + EndVert() + "']";

        // example: "["Paris"->"Berlin"]"
      }

      return name;
    }

   private:
    vert_t start_vert_;
    vert_t end_vert_;
    weight_t weight_ = 0;
  };

  std::vector<Edge> edges_;
  std::vector<vert_t> verts_;

  bool is_direct_ = true;
  bool is_weighted_ = false;

 public:
  friend std::ostream& operator<<(std::ostream& os,
                                  const Graph<vert_t, weight_t>::Edge& edge) {
    os << edge.Name();
    return os;
  }

 private:
  Graph(const std::vector<Edge>& edges) {
    if (edges.empty()) return;

    for (const auto& edge : edges) {
      if (edge.IsWeighted()) is_weighted_ = true;
      AddEdge_(edge);
    }

    if constexpr (std::is_arithmetic_v<vert_t>) {
      // кол-во вершин = максимальная вершина среди ребер, т.е. в этом случае
      // происходит заполнение вершин до наибольшей из них в списке ребер
      vert_t max_vert = edges[0].StartVert();

      for (const auto& edge : edges_) {
        max_vert = std::max(max_vert, edge.StartVert());
        max_vert = std::max(max_vert, edge.EndVert());
      }

      verts_.resize(max_vert + 1);
      std::iota(verts_.begin(), verts_.end(), 0);

    } else if constexpr (std::is_same_v<vert_t, std::string>)
      for (const auto& edge : edges_) {
        if (!Contains(Verts(), edge.StartVert()))
          verts_.push_back(edge.StartVert());

        if (!Contains(Verts(), edge.EndVert()))
          verts_.push_back(edge.EndVert());
      }
  }

  void AddEdge_(const Edge& edge) {
    AddVert(edge.StartVert());
    AddVert(edge.EndVert());

    if (!Contains(edges_, edge)) edges_.emplace_back(edge);

    if (edge.Weight() != 0) is_weighted_ = true;
  }

  static std::pair<vert_t, vert_t> ParseEdgeString_(
      const std::string& edge_str) {
    const size_t pos = edge_str.find("->");
    vert_t start_vert;
    vert_t end_vert;

    if (pos == std::string::npos)
      throw std::invalid_argument("EdgeString: invalid edge string format: " +
                                  edge_str);

    if constexpr (std::is_arithmetic_v<vert_t>) {
      try {
        start_vert = std::stoul(edge_str.substr(0, pos));
        end_vert = std::stoul(edge_str.substr(pos + 2));
      }

      catch (const std::exception&) {
        throw std::invalid_argument(
            "EdgeString: invalid edge string format "
            "(vertices should be numbers): " +
            edge_str);
      }
    }

    else if constexpr (std::is_same_v<vert_t, std::string>) {
      start_vert = edge_str.substr(0, pos);
      end_vert = edge_str.substr(pos + 2);
    }

    return {start_vert, end_vert};
  }

  auto GetEdgeIter_(const std::pair<vert_t, vert_t>& edge) const {
    auto [start_vert, end_vert] = edge;

    return std::find_if(
        edges_.begin(), edges_.end(),
        [start_vert, end_vert, this](const auto& e) {
          return (e.StartVert() == start_vert && e.EndVert() == end_vert) ||
                 (!IsDirected() && e.StartVert() == end_vert &&
                  e.EndVert() == start_vert);
        });
  }

  auto GetEdgeIter_(const std::pair<vert_t, vert_t>& edge) {
    auto [start_vert, end_vert] = edge;

    return std::find_if(
        edges_.begin(), edges_.end(),
        [start_vert, end_vert, this](const auto& e) {
          return (e.StartVert() == start_vert && e.EndVert() == end_vert) ||
                 (!IsDirected() && e.StartVert() == end_vert &&
                  e.EndVert() == start_vert);
        });
  }

  auto GetEdgeIter_(const std::tuple<vert_t, vert_t, weight_t>& edge) const {
    auto [start_vert, end_vert, weight] = edge;

    return std::find_if(
        edges_.begin(), edges_.end(),
        [start_vert, end_vert, weight, this](const auto& e) {
          return (e.StartVert() == start_vert && e.EndVert() == end_vert &&
                  e.Weight() == weight) ||
                 (!IsDirected() && e.StartVert() == end_vert &&
                  e.EndVert() == start_vert && e.Weight() == weight);
        });
  }
};

// MARK: operator<<

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
