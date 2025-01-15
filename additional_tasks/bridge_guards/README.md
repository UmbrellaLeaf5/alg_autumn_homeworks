# Задача: охранники речных мостов

Река имеет `N` контрольно-пропускных пунктов на левом берегу и `M` контрольно-пропускных пунктов на правом берегу. 

`P` мостов построены, соединяя контрольно-пропускные пункты через реку. На контрольно-пропускных пунктах необходимо разместить охрану, и охрана может защищать все мосты, на которых присутствует этот КПП. 

Для защиты одного моста может быть более одного охранника.

Найдите минимальное количество охранников, необходимое для защиты всех мостов через реку.

## Входные данные:

* Первая строка ввода состоит из 2 целых чисел, разделенных пробелами `N`
и `M` − количество КПП на левом и правом берегу реки соответственно (1 ≤ `N`, `M` ≤ 100)
* Вторая строка ввода состоит из одного целого числа `P` − общее количество мостов через реку (1 ≤ `P` ≤ 100). 
* Следующие `P` строк, каждая из которых состоит из 2 целых чисел, разделенных пробелами `u`, `v`, обозначающих, что между КПП `u` на левом берегу и КПП `v` на правом берегу есть мост (1 ≤ `u` ≤ `N`) (1 ≤ `v` ≤ `M`).

## Выходные данные:

* Одно целое число − минимальное количество охранников, необходимое для защиты всех мостов через реку.

## Пример:

### Вход:
```
4 3
4
1 3
1 2
2 2
4 1
```

### Выход:
```
3
```

### Решение:

#### Введение и моделирование графом:

Для решения задачи о часовых на реке, смоделируем ситуацию с помощью двудольного графа. 
Одна доля графа представляла пункты наблюдения на левом берегу, а другая – на правом берегу.

Ребра между долями обозначали мосты. Целью было найти минимальный набор вершин (пунктов), которые ‘покрывают’ все ребра (мосты). Это классическая задача поиска минимального вершинного покрытия. 

Известно, что в общем случае задача поиска минимального вершинного покрытия является NP-трудной, то есть не существует быстрого алгоритма для произвольных графов. Однако, для двудольных графов эту задачу можно решить за полиномиальное время, применив теорему Кёнига.

**Теорема Кёнига** утверждает, что в двудольном графе размер минимального вершинного покрытия равен размеру максимального паросочетания. 

*Паросочетание* – это набор ребер, которые не имеют общих вершин, то есть ни одна вершина не является концом более чем одного ребра в паросочетании. 

*Максимальное паросочетание* – это паросочетание с наибольшим возможным количеством ребер.

#### Максимальное паросочетание и алгоритм поиска максимального потока:

Для нахождения максимального паросочетания используем алгоритм поиска максимального потока. 

Чтобы применить этот алгоритм, преобразуем двудольный граф в сеть, добавив искусственный источник (`s`) и сток (`t = s + 1`). 

Источник (`s`) имел ребра единичной пропускной способности ко всем вершинам, представляющим пункты на левом берегу. Все вершины, представляющие пункты на правом берегу, имели ребра единичной пропускной способности к стоку (`t`). Рёбра между пунктами левого и правого берегов (представляющие мосты) также имели единичную пропускную способность. 

Это преобразование позволило свести задачу поиска максимального паросочетания к задаче поиска максимального потока в созданной сети.

Для поиска максимального потока используем модификацию алгоритма Эдмондса-Карпа, реализуя обход в ширину (BFS) для поиска увеличивающего пути. 

Функция `BFS` реализована следующим образом:
```C++
inline size_t BFS(size_t s, std::vector<ssize_t>& parent,
                  std::vector<std::vector<size_t>>& adj_list,
                  std::vector<std::vector<size_t>>& capacity) {
  parent[s] = -2;
  size_t n = parent.size();

  std::vector<bool> visited(n, false);
  visited[s] = true;

  std::queue<std::pair<size_t, size_t>> nodes;

  nodes.push({s, LLONG_MAX});

  while (!nodes.empty()) {
    std::pair<size_t, size_t> node_flow = nodes.front();
    size_t node = node_flow.first;
    size_t flow = node_flow.second;

    nodes.pop();

    for (size_t i = 0; i < adj_list[node].size(); i++) {
      size_t next = adj_list[node][i];

      if (visited[next] || capacity[node][i] == 0) continue;

      visited[next] = true;

      parent[next] = node;

      size_t new_flow = std::min(flow, capacity[node][i]);

      if (next == s + 1) return new_flow;
      nodes.push({next, new_flow});
    }
  }

  return 0;
}
```

В этой функции `parent` используется для отслеживания пути, `adj_list` - список смежности, `capacity` - матрица пропускных способностей. Функция возвращает пропускную способность найденного увеличивающего пути, или 0, если путь не найден.

#### Реализация алгоритма максимального потока:

Алгоритм максимального потока реализован в функции `MaxFlow`:
```C++
inline size_t MaxFlow(size_t s, std::vector<std::vector<size_t>>& adj_list,
                      std::vector<std::vector<size_t>>& capacity) {
  size_t flow = 0;
  std::vector<ssize_t> parent(adj_list.size(), -1);

  size_t new_flow = 0;

  while ((new_flow = BFS(s, parent, adj_list, capacity))) {
    flow += new_flow;
    size_t curr = s + 1;

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

  return flow;
}

```

Функция `MaxFlow` инициализирует поток в 0, и пока находит увеличивающие пути с помощью `BFS`, наращивает поток и обновляет пропускные способности остаточных ребер. Возвращает величину максимального потока.

#### Преобразование двудольного графа в сеть и нахождение максимального паросочетания

Преобразование двудольного графа в сеть и использование алгоритма максимального потока для нахождения максимального паросочетания реализовано в функции `MaximumBipartiteMatching`:
```C++
inline size_t MaximumBipartiteMatching(size_t n, size_t m,
                                       Graph<size_t>& bipartite_graph) {
  std::vector<std::vector<std::pair<size_t, size_t>>> bipartite_edges_stack(
      n + m + 3);
  std::vector<std::vector<size_t>> adj_list(n + m + 3);
  std::vector<std::vector<size_t>> capacity(n + m + 3);

  for (size_t i = 0; i < bipartite_graph.EdgesAmount(); i++) {
    size_t u = StartVertFromTuple(bipartite_graph.Edges()[i]);
    size_t v = EndVertFromTuple(bipartite_graph.Edges()[i]);

    v += n;

    bipartite_edges_stack[u].push_back({v, 1});
    bipartite_edges_stack[v].push_back({u, 0});  
  }

  for (size_t i = 1; i <= n; i++) {
    bipartite_edges_stack[n + m + 1].push_back({i, 1});
    bipartite_edges_stack[i].push_back({n + m + 1, 0});
  }
  for (size_t i = 1; i <= m; i++) {
    bipartite_edges_stack[i + n].push_back({n + m + 2, 1});
    bipartite_edges_stack[n + m + 2].push_back({i + n, 0});
  }

  for (size_t i = 1; i <= n + m + 2; i++)
    sort(bipartite_edges_stack[i].begin(), bipartite_edges_stack[i].end());

  for (size_t i = 1; i <= n + m + 2; i++)
    for (size_t j = 0; j < bipartite_edges_stack[i].size(); j++) {
      adj_list[i].push_back(bipartite_edges_stack[i][j].first);
      capacity[i].push_back(bipartite_edges_stack[i][j].second);
    }

  return MaxFlow(n + m + 1, adj_list, capacity);
}

```

В этой функции двудольный граф `bipartite_graph` преобразуется в сеть с помощью `bipartite_edges_stack`, добавляется источник (`n + m + 1`) и сток (`n + m + 2`), затем строится матрица смежности `adj_list` и матрица пропускных способностей `capacity` и вызывается функция MaxFlow для вычисления максимального потока.

#### Вызов функции решения и заключение:

Функция `Solution` считывает входные данные и выводит результат:

```C++
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
```

В результате, величина максимального потока, возвращаемая функцией `MaximumBipartiteMatching` и являющаяся размером максимального паросочетания, дает размер минимального вершинного покрытия, что и являлось ответом к задаче – минимальное количество необходимых часовых.