#include <iostream>
#include <vector>
#include <ranges>
#include <queue>
#include <map>

using Graph = std::vector<std::vector<int>>;

std::pair<std::vector<int>, std::vector<int>> BFS(int startVertex, const Graph& graph)
{
    std::vector<int> d(graph.size(), INT_MAX);
    std::vector<int> pi(graph.size(), -1);

    d[startVertex] = 0;

    std::queue<int> Q;
    Q.push(startVertex);
    while (!Q.empty())
    {
        auto v = Q.front();
        Q.pop();

        for (auto& u : graph[v])
        {
            if (d[u] > d[v] + 1)
            {
                d[u] = d[v] + 1;
                pi[u] = v;
                Q.push(u);
            }
        }
    }

    return { std::move(d), std::move(pi) };
}

bool IsConnected(const Graph& graph)
{
    auto [d, pi] = BFS(0, graph);
    return std::ranges::all_of(d, [](const auto& distance) { return distance < INT_MAX; });
}

int CountOfConnectivityComponents(const Graph& graph)
{
    auto count = 0;
    std::vector<bool> used(graph.size(), false);
    for (int i = 0; i < graph.size(); ++i)
    {
        if (!used[i])
        {
            auto [d, pi] = BFS(i, graph);
            ++count;
            for (int j = 0; j < d.size(); ++j)
                if (!used[j]) used[j] = d[j] != INT_MAX;
        }
    }

    return count;
}


bool IsBipartite(const Graph& graph)
{
    std::vector<int> used(graph.size(), 0);

    for (int i = 0; i < graph.size(); ++i)
    {
        if (used[i] == 0)
        {
            auto [d, pi] = BFS(i, graph);
            used[i] = 1;

            for (int j = 0; j < d.size(); ++j)
            {
                if (d[j] != INT_MAX && d[j] != 0)
                {
                    if (used[j] == 0)
                    {
                        used[j] = used[pi[j]] == 1 ? 2 : 1;
                    }
                    else if (used[j] == used[pi[j]]) return false;
                }
            }
        }
    }

    return true;
}

int main()
{
    return 0;
}
