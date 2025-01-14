#pragma once

#include <functional>
#include <stack>

#include "graph.hpp"

// FIXME: необязательно vert_t, может быть любое сравнимое

/**
 * @brief Класс, представляющий неявное декартово дерево (Implicit Treap).
 * (состоит из индексов)
 *
 * @details Реализует неявное декартово дерево, где ключи являются неявными
 * (индексами), а приоритеты задаются пользователем. Дерево используется для
 * построения графа.
 *
 * @tparam vert_t: тип приоритетов.
 */
template <AllowedVertType vert_t>
class ImplicitIndexTreap {
 private:
  /// @brief Структура, представляющая узел декартова дерева.
  struct TreapNode {
    /// @brief Неявный ключ узла (индекс).
    size_t key;
    /// @brief Приоритет узла (значение в массиве).
    vert_t priority;

    /// @brief Указатель на левого потомка.
    TreapNode* left;
    /// @brief Указатель на правого потомка.
    TreapNode* right;

    /// @brief Минимальный размер поддерева, корнем которого является узел.
    size_t size;

    /**
     * @brief Конструктор узла декартова дерева.
     *
     * @param key: неявный ключ узла.
     * @param priority: приоритет узла.
     */
    TreapNode(size_t key, const vert_t& priority)
        : key(key),
          priority(priority),
          left(nullptr),
          right(nullptr),
          size(1) {}

    ~TreapNode() {
      if (left) delete left;
      if (right) delete right;
    }

    /**
     * @brief Обновляет размер поддерева узла.
     *
     * @details Размер вычисляется как сумма размеров левого и правого
     * поддеревьев + 1 (текущий узел).
     */
    void UpdateSize() {
      size = (left ? left->size : 0) + (right ? right->size : 0) + 1;
    }
  };

  // @brief Корень декартова дерева.
  TreapNode* root_;

  Graph<size_t> graph_;

 public:
  /**
   * @brief Конструктор неявного декартова дерева.
   *
   * @details Строит декартово дерево на основе переданных приоритетов,
   * используя стек. Каждый узел получает ключ-индекс, соответствующий его
   * положению в векторе. После построения дерева строится граф, где ребрами
   * являются отношения родитель-потомок.
   *
   * @throw `std::invalid_argument("ImplicitIndexTreap: priorities can not be
   * empty.")`.
   *
   * @param priorities: вектор приоритетов для узлов дерева.
   */
  ImplicitIndexTreap(const std::vector<vert_t>& priorities) {
    if (priorities.empty())
      throw std::invalid_argument(
          "ImplicitIndexTreap: priorities can not be empty.");

    // @brief Стек для построения дерева.
    std::stack<std::tuple<size_t, vert_t, TreapNode*>> stack;

    // @brief Корень дерева.
    TreapNode* root = nullptr;

    for (size_t i = 0; i < priorities.size(); i++) {
      // @brief Новый узел дерева.
      TreapNode* new_node = new TreapNode(i, priorities[i]);

      /// @brief Указатель на последнего левого ребенка.
      TreapNode* last = nullptr;

      // пока стек не пуст и приоритет узла на вершине стека больше
      while (!stack.empty() &&
             std::get<2>(stack.top())->priority > new_node->priority) {
        last = std::get<2>(stack.top());  // запоминаем верхний элемент стека
        stack.pop();

        // обновляем размер удаленного узла
        if (last != nullptr) last->UpdateSize();
      }

      // если был левый ребенок, то делаем его левым ребенком нового узла
      if (last != nullptr) new_node->left = last;

      // если стек не пустой, то новый узел делаем правым ребенком верхнего
      // элемента стека
      if (!stack.empty()) std::get<2>(stack.top())->right = new_node;

      //  новый узел в стек, обновляем размер текущего узла
      stack.push({i, priorities[i], new_node});
      if (new_node != nullptr) new_node->UpdateSize();
    }

    // пока в стеке больше одного элемента (остался только корень)
    while (stack.size() > 1) stack.pop();

    if (!stack.empty()) root = std::get<2>(stack.top());

    root_ = root;

    /**
     * @brief Лямбда функция для обхода дерева и добавления ребер в граф.
     *
     * @details Обходит дерево в порядке "корень-лево-право", добавляя ребро из
     * родительской вершины в текущую вершину, если родительская вершина не
     * является "фиктивной" (extra_value).
     *
     * @param node: текущий узел дерева, который нужно обработать.
     * @param parent_key: ключ родительской вершины текущего узла.
     */
    std::function<void(TreapNode*, ssize_t)> Traverse =
        [&](TreapNode* node, ssize_t parent_key) -> void {
      if (!node) return;

      // ребро из родителя в текущий узел
      if (parent_key != -1) graph_.AddEdge({parent_key, node->key});

      // рекурсивный обход левого и правого поддеревьев
      Traverse(node->left, node->key);
      Traverse(node->right, node->key);
    };

    // обходим дерево и строим граф
    Traverse(root, -1);
    graph_.MakeUndirected();
  }

  /**
   * @brief Деструктор неявного декартова дерева.
   *
   * @details Освобождает память, выделенную для всех узлов дерева.
   */
  ~ImplicitIndexTreap() {
    if (root_) delete root_;
  }

  /**
   * @brief Возвращает граф, построенный на основе неявного декартового дерева.
   *
   * @details Состоит из индексов изначального массива.
   *
   * @return `Graph<size_t>`: граф.
   */
  const Graph<size_t> GetGraph() const { return graph_; }

  /**
   * @brief Возвращает ключ корня дерева.
   *
   * @return `size_t`: ключ корня.
   */
  size_t Root() const { return root_->key; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const ImplicitIndexTreap& treap) {
    std::function<void(ImplicitIndexTreap::TreapNode*, std::string)> PrintNode =
        [&](ImplicitIndexTreap::TreapNode* node, std::string indent) -> void {
      if (!node) return;

      os << indent << node->key << ": [prior: " << node->priority
         << " , size: " << node->size << "]" << std::endl;

      indent += "  ";

      if (node->left) PrintNode(node->left, indent);
      if (node->right) PrintNode(node->right, indent);
    };

    PrintNode(treap.root_, "");

    return os;
  }
};