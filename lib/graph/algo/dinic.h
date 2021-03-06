#pragma once

#include <limits>
#include "../graph.h"
#include "../../collections/arr.h"
#include "../../collections/queue.h"
#include "../../recursive_function.h"

template <class Edge, typename C>
C dinic(Graph<Edge>& graph, int source, int destination) {
    arri dist(graph.vertexCount);
    arri nextEdge(graph.vertexCount);
    C inf = numeric_limits<C>::max();
    C totalFlow = 0;
    que<int> q;
    auto edgeDistances = [&]() {
        fill(all(dist), -1);
        dist[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (auto edge : graph[current]) {
                if (edge->capacity != 0) {
                    int next = edge->to;
                    if (dist[next] == -1) {
                        dist[next] = dist[current] + 1;
                        q.push(next);
                    }
                }
            }
        }
    };
    RecursiveFunction dinicImpl = [&](const auto& self, int source, C flow) -> C {
        if (source == destination) {
            return flow;
        }
        if (flow == 0 || dist[source] == dist[destination]) {
            return 0;
        }
        C totalPushed = 0;
        while (nextEdge[source] < graph[source].size()) {
            auto edge = graph[source][nextEdge[source]];
            if (edge->capacity != 0 && dist[edge->to] == dist[source] + 1) {
                C pushed = self(edge->to, min(flow, edge->capacity));
                if (pushed != 0) {
                    edge->push(pushed);
                    flow -= pushed;
                    totalPushed += pushed;
                    if (flow == 0) {
                        return totalPushed;
                    }
                }
            }
            nextEdge[source]++;
        }
        return totalPushed;
    };

    while (true) {
        edgeDistances();
        if (dist[destination] == -1) {
            break;
        }
        fill(all(nextEdge), 0);
        totalFlow += dinicImpl(source, inf);
    }
    return totalFlow;
}
