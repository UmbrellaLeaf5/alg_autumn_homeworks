#include <climits>
#include <queue>

#include "graph.hpp"

/**
 * @brief Функция BFS (поиск в ширину) для нахождения увеличивающего пути в
 * сети.
 *
 * @param s: начальная вершина (источник) в сети.
 * @param parent: массив, хранящий родительские вершины для восстановления пути.
 * @param adj_list: список смежности графа.
 * @param capacity: матрица пропускных способностей ребер.
 *
 * @return `size_t`: пропускная способность найденного увеличивающего пути, или
 * 0, если путь не найден.
 */
inline size_t BFS(size_t s, std::vector<ssize_t>& parent,
                  std::vector<std::vector<size_t>>& adj_list,
                  std::vector<std::vector<size_t>>& capacity) {
  // инициализация родительского массива и массива посещенных вершин

  parent[s] = -2;  // отмечаем начальную вершину как посещенную, значение -2
                   // используется для обозначения источника
  size_t n = parent.size();

  std::vector<bool> visited(n, false);
  visited[s] = true;

  // используем очередь для обхода графа в ширину
  std::queue<std::pair<size_t, size_t>> nodes;

  // добавляем начальную вершину с максимальным потоком
  nodes.push({s, LLONG_MAX});

  // обходим граф до тех пор, пока очередь не пуста
  while (!nodes.empty()) {
    // извлекаем вершину и её текущий поток из очереди
    std::pair<size_t, size_t> node_flow = nodes.front();
    size_t node = node_flow.first;
    size_t flow = node_flow.second;

    nodes.pop();

    // проверяем все ребра, исходящие из текущей вершины
    for (size_t i = 0; i < adj_list[node].size(); i++) {
      size_t next = adj_list[node][i];  // следующая вершина

      // если следующая вершина уже посещена или пропускная способность ребра
      // равна 0, то пропускаем ребро
      if (visited[next] || capacity[node][i] == 0) continue;

      // отмечаем следующую вершину как посещенную
      visited[next] = true;

      // записываем родительскую вершину
      parent[next] = node;

      // вычисляем новый поток как min из текущего потока и пропускной
      // способности ребра
      size_t new_flow = std::min(flow, capacity[node][i]);

      // если достигли стока, то возвращаем найденный поток
      if (next == s + 1) return new_flow;
      // иначе добавляем следующую вершину в очередь с новым потоком
      nodes.push({next, new_flow});
    }
  }

  // увеличивающий путь не найден, возвращаем 0
  return 0;
}

/**
 * @brief Функция для вычисления максимального потока в сети.
 *
 * @param s: начальная вершина (источник) в сети.
 * @param adj_list: список смежности графа.
 * @param capacity: матрица пропускных способностей ребер.
 *
 * @return `size_t`: величина максимального потока в сети.
 */
inline size_t MaxFlow(size_t s, std::vector<std::vector<size_t>>& adj_list,
                      std::vector<std::vector<size_t>>& capacity) {
  size_t flow = 0;  // инициализируем максимальный поток нулем
  std::vector<ssize_t> parent(adj_list.size(), -1);  // родительский массив

  size_t new_flow = 0;

  // повторяем поиск увеличивающих путей до тех пор, пока они находятся
  while ((new_flow = BFS(s, parent, adj_list, capacity))) {
    // добавляем найденный поток к максимальному потоку
    flow += new_flow;

    size_t curr = s + 1;  // начинаем с стока

    // обновляем пропускные способности ребер по найденному пути
    while (curr != s) {
      size_t prev = parent[curr];
      size_t idx = (find(adj_list[prev].begin(), adj_list[prev].end(), curr) -
                    adj_list[prev].begin());
      capacity[prev][idx] -= new_flow;

      idx = (find(adj_list[curr].begin(), adj_list[curr].end(), prev) -
             adj_list[curr].begin());
      capacity[curr][idx] += new_flow;

      curr = prev;
    }
  }

  return flow;  // максимальный поток
}

/**
 * @brief Функция для вычисления максимального паросочетания в двудольном графе.
 *
 * @param n: количество вершин в первой доле графа.
 * @param m: количество вершин во второй доле графа.
 * @param bipartite_graph: объект, представляющий двудольный граф.
 *
 * @return `size_t`: размер максимального паросочетания.
 */
inline size_t MaximumBipartiteMatching(size_t n, size_t m,
                                       Graph<size_t>& bipartite_graph) {
  // вспомогательные структуры данных для алгоритма Форда-Фалкерсона
  std::vector<std::vector<std::pair<size_t, size_t>>> bipartite_edges_stack(
      n + m + 3);
  std::vector<std::vector<size_t>> adj_list(n + m + 3);
  std::vector<std::vector<size_t>> capacity(n + m + 3);

  // преобразуем двудольный граф в сеть

  for (size_t i = 0; i < bipartite_graph.EdgesAmount(); i++) {
    size_t u = StartVertFromTuple(bipartite_graph.Edges()[i]);
    size_t v = EndVertFromTuple(bipartite_graph.Edges()[i]);

    v += n;  // смещаем номера вершин второй доли

    bipartite_edges_stack[u].push_back({v, 1});
    bipartite_edges_stack[v].push_back(
        {u, 0});  // обратное ребро с пропускной способностью 0
  }

  // добавляем источник и сток
  for (size_t i = 1; i <= n; i++) {
    bipartite_edges_stack[n + m + 1].push_back({i, 1});
    bipartite_edges_stack[i].push_back({n + m + 1, 0});
  }
  for (size_t i = 1; i <= m; i++) {
    bipartite_edges_stack[i + n].push_back({n + m + 2, 1});
    bipartite_edges_stack[n + m + 2].push_back({i + n, 0});
  }

  // сортируем ребра для удобства работы
  for (size_t i = 1; i <= n + m + 2; i++)
    sort(bipartite_edges_stack[i].begin(), bipartite_edges_stack[i].end());

  // списки смежности и пропускных способностей
  for (size_t i = 1; i <= n + m + 2; i++)
    for (size_t j = 0; j < bipartite_edges_stack[i].size(); j++) {
      adj_list[i].push_back(bipartite_edges_stack[i][j].first);
      capacity[i].push_back(bipartite_edges_stack[i][j].second);
    }

  // максимальный поток, который равен максимальному паросочетанию
  return MaxFlow(n + m + 1, adj_list, capacity);
}

inline void Solution(std::istream& is = std::cin,
                     std::ostream& os = std::cout) {
  size_t n, m;
  is >> n >> m;

  size_t p;
  is >> p;

  Graph<size_t> bipartite_graph;

  for (size_t i = 0; i < p; i++) {
    size_t u, v;
    is >> u >> v;

    bipartite_graph.AddEdge({u, v});
  }

  os << MaximumBipartiteMatching(n, m, bipartite_graph) << std::endl;
  return;
}