#include <algorithm>
#include <fstream>
#include <sstream>
#include <stack>

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
 * @brief Итеративно добавляет ребра в граф на основе смежности ячеек.
 * @param graph: граф, в который нужно добавить ребра.
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

/**
 * @brief Разбирает входные данные лабиринта из стандартного ввода и создает
 * направленный граф.
 * @return PetyaAndVasyaGraphs, содержащий графы для обоих игроков.
 */
inline PetyaAndVasyaGraphs ParseMaze(std::istream& is = std::cin) {
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
  AddEdgesIterative(maze.petya.graph,
                    FindCell(graph.Verts(), petya_start_i, petya_start_j));
  maze.petya.graph.MakeDirected();

  maze.vasya.graph = graph;
  AddEdgesIterative(maze.vasya.graph,
                    FindCell(graph.Verts(), vasya_start_i, vasya_start_j));
  maze.vasya.graph.MakeDirected();

  return maze;
}

/// @brief Решает игру в лабиринте и определяет победителя.
void Solution(std::istream& is = std::cin, std::ostream& os = std::cout) {
  try {
    auto maze = ParseMaze(is);

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
  } catch (...) {
    throw std::invalid_argument("Invalid enter format!");
  }
}