#pragma once

#include <functional>
#include <stack>

#include "graph.hpp"

template <AllowedVertType vert_t, AllowedWeightType weight_t>
class ImplicitTreap : public Graph<vert_t, weight_t> {
 private:
  struct TreapNode {
    size_t key;
    vert_t priority;

    TreapNode* left;
    TreapNode* right;

    size_t size;

    TreapNode(size_t key, vert_t priority)
        : key(key),
          priority(priority),
          left(nullptr),
          right(nullptr),
          size(1) {}
  };

  size_t TreapSizeFromNode(TreapNode* node) const {
    return node ? node->size : 0;
  }

  void UpdateTreapSizeFromNode(TreapNode* node) const {
    if (node)
      node->size =
          TreapSizeFromNode(node->left) + TreapSizeFromNode(node->right) + 1;
  }

  TreapNode* root_;

 public:
  ImplicitTreap(const std::vector<vert_t>& priorities) {
    std::stack<std::tuple<size_t, size_t, TreapNode*>> stack;
    TreapNode* root = nullptr;
    std::vector<std::pair<vert_t, vert_t>> edges;

    for (size_t i = 0; i < priorities.size(); i++) {
      TreapNode* new_node = new TreapNode(i, priorities[i]);
      TreapNode* last = nullptr;

      while (!stack.empty() &&
             std::get<2>(stack.top())->priority > new_node->priority) {
        last = std::get<2>(stack.top());
        stack.pop();

        if (last != nullptr) UpdateTreapSizeFromNode(last);
      }

      if (last != nullptr) new_node->left = last;

      if (!stack.empty()) std::get<2>(stack.top())->right = new_node;

      stack.push({i, priorities[i], new_node});
      if (new_node != nullptr) UpdateTreapSizeFromNode(new_node);
    }

    while (stack.size() > 1) stack.pop();

    if (!stack.empty()) root = std::get<2>(stack.top());

    root_ = root;

    vert_t extra_value;
    if constexpr (std::is_same_v<vert_t, std::string>) {
      extra_value = "_____extra_value_____";
      while (Contains(priorities, extra_value))
        extra_value = "_" + extra_value + "_";

    } else {
      extra_value = -1;
      for (const auto& value : priorities)
        if (value <= extra_value) extra_value = value - 1;
    }

    std::function<void(TreapNode*, vert_t)> traverse =
        [&](TreapNode* node, vert_t parent_priority) {
          if (node == nullptr) return;

          if (parent_priority != extra_value)
            this->AddEdge({parent_priority, node->priority});

          traverse(node->left, node->priority);
          traverse(node->right, node->priority);
        };

    traverse(root, extra_value);
    this->MakeUndirected();
  }

  Graph<vert_t, weight_t> GetGraph() const { return *this; }
  size_t Root() const { return root_->key; }
};