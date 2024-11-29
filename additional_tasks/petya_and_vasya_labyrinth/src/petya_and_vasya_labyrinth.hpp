#include <algorithm>
#include <fstream>
#include <sstream>
#include <stack>

#include "a_star.hpp"
#include "dag_relaxation.hpp"

namespace {

/**
 * @brief Находит ячейку в заданном векторе строк, соответствующую определенному
 * суффиксу.
 * @param strings: вектор строк для поиска.
 * @param i: индекс.
 * @param j: индекс.
 * @return std::string: строка, содержащая суффикс, или пустая строка, если
 * совпадений не найдено.
 */
inline std::string FindCell(const std::vector<std::string>& strings, size_t i,
                            size_t j) {
  std::string suffix = "_" + std::to_string(i) + "_" + std::to_string(j);

  auto it =
      std::find_if(strings.begin(), strings.end(), [&](const std::string& str) {
        return str.find(suffix) != std::string::npos;
      });

  return it == strings.end() ? "" : *it;
}

/**
 * @brief Добавляет ребра в граф на основе смежности ячеек.
 * @param graph: граф, в который нужно добавить ребра (НЕНАПРАВЛЕННЫЙ).
 */
inline void AddEdgesNonDirected(Graph<std::string, long>& graph) {
  for (const auto& vert : graph.Verts()) {
    size_t j = std::stoll(vert.substr(vert.rfind('_') + 1));
    size_t i = std::stoll(vert.substr(
        vert.rfind('_', vert.rfind('_') - 1) + 1,
        vert.rfind('_') - vert.rfind('_', vert.rfind('_') - 1) - 1));

    std::string up_vert = "", down_vert = "";
    auto right_vert = FindCell(graph.Verts(), i + 1, j);
    auto left_vert = FindCell(graph.Verts(), i, j + 1);

    if (i != 0 && j != 0) {
      up_vert = FindCell(graph.Verts(), i - 1, j);
      down_vert = FindCell(graph.Verts(), i, j - 1);
    }

    for (const auto& another_vert : {up_vert, down_vert, right_vert, left_vert})
      if (!another_vert.empty() && !graph.ContainsEdge({another_vert, vert}))
        graph.AddEdge(vert, another_vert);
  }
}

/**
 * @brief Итеративно добавляет ребра в граф на основе смежности ячеек.
 * @param graph: граф, в который нужно добавить ребра (НАПРАВЛЕННЫЙ).
 * @param vert: начальная вершина для обхода.
 */
inline void AddEdgesIterative(Graph<std::string, long>& graph,
                              const std::string& vert) {
  std::stack<std::string> s;
  s.push(vert);

  while (!s.empty()) {
    std::string current_vert = s.top();
    s.pop();

    size_t j = std::stoll(current_vert.substr(current_vert.rfind('_') + 1));
    size_t i = std::stoll(current_vert.substr(
        current_vert.rfind('_', current_vert.rfind('_') - 1) + 1,
        current_vert.rfind('_') -
            current_vert.rfind('_', current_vert.rfind('_') - 1) - 1));

    std::string up_vert = "", down_vert = "";
    auto right_vert = FindCell(graph.Verts(), i + 1, j);
    auto left_vert = FindCell(graph.Verts(), i, j + 1);

    if (i != 0 && j != 0) {
      up_vert = FindCell(graph.Verts(), i - 1, j);
      down_vert = FindCell(graph.Verts(), i, j - 1);
    }

    for (const auto& another_vert : {up_vert, down_vert, right_vert, left_vert})
      if (!another_vert.empty() &&
          !graph.ContainsEdge({another_vert, current_vert})) {
        graph.AddEdge(current_vert, another_vert);
        if (another_vert[0] != 'E') s.push(another_vert);
      }
  }
}

}  // namespace

struct PetyaAndVasyaGraphs {
  struct {
    Graph<std::string, long> graph;
    std::string vert;
    std::vector<std::string> exits;
  } petya, vasya;
};

enum class SolutionType { ComplicatedBad, Good };

/**
 * @brief Разбирает входные данные лабиринта из стандартного ввода и создает
 * направленный граф.
 * @return `PetyaAndVasyaGraphs`, содержащий графы для обоих игроков.
 */
inline PetyaAndVasyaGraphs ParseMaze(
    std::istream& is = std::cin,
    SolutionType sol_type = SolutionType::ComplicatedBad) {
  Graph<std::string, long> graph;
  std::vector<std::string> console_maze;
  std::string line;

  PetyaAndVasyaGraphs maze;
  size_t petya_start_i = -1, petya_start_j = -1, vasya_start_i = -1,
         vasya_start_j = -1;

  // считывание лабиринта
  while (std::getline(is, line) && !line.empty()) console_maze.push_back(line);

  // добавление вершин
  for (size_t i = 0; i < console_maze.size(); i++) {
    const auto& line = console_maze[i];

    for (size_t j = 0; j < line.size(); j++) {
      const auto& cell = line[j];
      std::stringstream cell_name_stream;

      if (cell == 'P') {
        petya_start_i = i, petya_start_j = j;
        maze.petya.vert = "E_" + std::to_string(i) + "_" + std::to_string(j);
        maze.vasya.exits.push_back(maze.petya.vert);

        cell_name_stream << 'E';

      } else if (cell == 'V') {
        vasya_start_i = i, vasya_start_j = j;
        maze.vasya.vert = "E_" + std::to_string(i) + "_" + std::to_string(j);
        maze.petya.exits.push_back(maze.vasya.vert);

        cell_name_stream << 'E';
      }

      else if (cell == '0') {
        if (j == 0 || i == 0 || j == (line.size() - 1) ||
            i == (console_maze.size() - 1)) {
          auto vert = "E_" + std::to_string(i) + "_" + std::to_string(j);

          maze.vasya.exits.push_back(vert);
          maze.petya.exits.push_back(vert);

          cell_name_stream << 'E';
        }

        else
          cell_name_stream << 'C';
      }

      if (!cell_name_stream.str().empty()) {
        cell_name_stream << "_" << i << "_" << j;

        graph.AddVert(cell_name_stream.str());
      }
    }
  }

  // добавление ребер
  maze.petya.graph = graph;
  maze.vasya.graph = graph;

  switch (sol_type) {
    case SolutionType::ComplicatedBad: {
      AddEdgesIterative(maze.petya.graph,
                        FindCell(graph.Verts(), petya_start_i, petya_start_j));

      AddEdgesIterative(maze.vasya.graph,
                        FindCell(graph.Verts(), vasya_start_i, vasya_start_j));

      maze.petya.graph.MakeDirected();
      maze.vasya.graph.MakeDirected();
      break;
    }

    case SolutionType::Good: {
      AddEdgesNonDirected(maze.petya.graph);
      maze.petya.graph.MakeUndirected();

      maze.vasya.graph = maze.petya.graph;
      break;
    }
  }

  return maze;
}

/// @brief Хорошо решает задачу: "Петя и Вася в страшном лабиринте".
void GoodSolution(std::istream& is = std::cin, std::ostream& os = std::cout) {
  try {
    auto maze = ParseMaze(is, SolutionType::Good);

    long petya_min = std::numeric_limits<long>::max();
    long vasya_min = std::numeric_limits<long>::max();

    auto manhattan_heuristic = [](const std::string& vert_1,
                                  const std::string& vert_2) {
      auto extract_coords = [](const std::string& vert) {
        size_t j = std::stoll(vert.substr(vert.rfind('_') + 1));
        size_t i = std::stoll(vert.substr(
            vert.rfind('_', vert.rfind('_') - 1) + 1,
            vert.rfind('_') - vert.rfind('_', vert.rfind('_') - 1) - 1));

        return std::make_pair(i, j);
      };

      // в матрице (лабиринте) координаты положительные, модули не нужны
      return extract_coords(vert_1).first - extract_coords(vert_2).first +
             extract_coords(vert_1).second - extract_coords(vert_2).second;
    };

    // (размеры будут одинаковые)
    for (size_t i = 0; i < maze.petya.exits.size(); i++) {
      petya_min = std::min(
          petya_min,
          AStar(maze.petya.vert, maze.petya.exits[i], maze.petya.graph,
                std::function<long(const std::string&, const std::string&)>(
                    manhattan_heuristic)));

      vasya_min = std::min(
          vasya_min,
          AStar(maze.vasya.vert, maze.vasya.exits[i], maze.vasya.graph,
                std::function<long(const std::string&, const std::string&)>(
                    manhattan_heuristic)));
    }

    if (vasya_min < petya_min)
      os << "Vasya! with " << vasya_min << std::endl;

    else if (vasya_min == petya_min)
      if (vasya_min != std::numeric_limits<long>::max())
        os << "Draw! with " << vasya_min << std::endl;
      else
        os << "Deadlock! Valery!" << std::endl;

    else
      os << "Petya! with " << petya_min << std::endl;

  } catch (const std::exception&) {
    throw std::invalid_argument("Invalid enter format!");
  }
}

/// @brief Сложно и плохо решает задачу: "Петя и Вася в страшном лабиринте".
void ComplicatedBadSolution(std::istream& is = std::cin,
                            std::ostream& os = std::cout) {
  try {
    auto maze = ParseMaze(is, SolutionType::ComplicatedBad);

    auto petya_relax = DAGRelaxation(maze.petya.graph, maze.petya.vert);
    auto vasya_relax = DAGRelaxation(maze.vasya.graph, maze.vasya.vert);

    long petya_min = std::numeric_limits<long>::max();
    long vasya_min = std::numeric_limits<long>::max();

    // (размеры будут одинаковые)
    for (size_t i = 0; i < maze.petya.exits.size(); i++) {
      petya_min = std::min(petya_min, petya_relax[maze.petya.exits[i]]);
      vasya_min = std::min(vasya_min, vasya_relax[maze.vasya.exits[i]]);
    }

    if (vasya_min < petya_min)
      os << "Vasya! with " << vasya_min << std::endl;

    else if (vasya_min == petya_min)
      if (vasya_min != std::numeric_limits<long>::max())
        os << "Draw! with " << vasya_min << std::endl;
      else
        os << "Deadlock! Valery!" << std::endl;

    else
      os << "Petya! with " << petya_min << std::endl;
  } catch (const std::exception&) {
    throw std::invalid_argument("Invalid enter format!");
  }
}
