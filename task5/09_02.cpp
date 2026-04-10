#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <vector>

class Tree
{
public:
    struct Node
    {
        explicit Node(int value)
            : value(value)
        {
        }

        ~Node()
        {
            std::cout << "destroy node " << value << '\n';
        }

        int value = 0;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;
    };

    std::shared_ptr<Node> root;

    void traverse_v1() const
    {
        std::vector<int> const order = bfs();
        std::cout << "bfs:";
        for (int value : order)
        {
            std::cout << ' ' << value;
        }
        std::cout << '\n';
    }

    void traverse_v2() const
    {
        std::vector<int> const order = dfs();
        std::cout << "dfs:";
        for (int value : order)
        {
            std::cout << ' ' << value;
        }
        std::cout << '\n';
    }

    std::vector<int> bfs() const
    {
        std::vector<int> order;
        if (!root)
        {
            return order;
        }

        std::queue<std::shared_ptr<Node>> queue;
        queue.push(root);

        while (!queue.empty())
        {
            std::shared_ptr<Node> current = queue.front();
            queue.pop();
            order.push_back(current->value);

            if (current->left)
            {
                queue.push(current->left);
            }
            if (current->right)
            {
                queue.push(current->right);
            }
        }

        return order;
    }

    std::vector<int> dfs() const
    {
        std::vector<int> order;
        if (!root)
        {
            return order;
        }

        std::stack<std::shared_ptr<Node>> stack;
        stack.push(root);

        while (!stack.empty())
        {
            std::shared_ptr<Node> current = stack.top();
            stack.pop();
            order.push_back(current->value);

            if (current->right)
            {
                stack.push(current->right);
            }
            if (current->left)
            {
                stack.push(current->left);
            }
        }

        return order;
    }
};

std::shared_ptr<Tree::Node> make_node(int value)
{
    return std::make_shared<Tree::Node>(value);
}

int main()
{
    {
        Tree tree;
        tree.root = make_node(1);
        tree.root->left = make_node(2);
        tree.root->right = make_node(3);
        tree.root->left->parent = tree.root;
        tree.root->right->parent = tree.root;

        tree.root->left->left = make_node(4);
        tree.root->left->right = make_node(5);
        tree.root->right->left = make_node(6);
        tree.root->right->right = make_node(7);

        tree.root->left->left->parent = tree.root->left;
        tree.root->left->right->parent = tree.root->left;
        tree.root->right->left->parent = tree.root->right;
        tree.root->right->right->parent = tree.root->right;

        std::vector<int> const bfs = tree.bfs();
        std::vector<int> const dfs = tree.dfs();

        if (bfs != std::vector<int>({1, 2, 3, 4, 5, 6, 7}))
        {
            std::cerr << "Unexpected BFS order\n";
            return 1;
        }
        if (dfs != std::vector<int>({1, 2, 4, 5, 3, 6, 7}))
        {
            std::cerr << "Unexpected DFS order\n";
            return 2;
        }

        tree.traverse_v1();
        tree.traverse_v2();
    }

    std::cout << "09.02: tree demo completed\n";
    return 0;
}
