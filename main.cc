#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <algorithm>

struct Point {
    int row, col;
    int distance;

    // Оператор сравнения для приоритетной очереди
    bool operator>(const Point& other) const {
        return distance > other.distance;
    }
};

bool isValid(int row, int col, int rows, int cols) {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

std::vector<Point> findShortestPath(const std::vector<std::vector<int>>& maze, Point start, Point end) {
    int rows = maze.size();
    int cols = maze[0].size();
    const int INF = std::numeric_limits<int>::max();

    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::vector<std::vector<int>> distances(rows, std::vector<int>(cols, INF));
    std::vector<std::vector<Point>> predecessors(rows, std::vector<Point>(cols, {-1, -1, -1}));

    std::priority_queue<Point, std::vector<Point>, std::greater<Point>> pq;

    distances[start.row][start.col] = 0;
    pq.push({start.row, start.col, 0});

    while (!pq.empty()) {
        Point current = pq.top();
        pq.pop();

        if (current.row == end.row && current.col == end.col) {
            break;
        }

        for (const auto& [dr, dc] : directions) {
            int newRow = current.row + dr;
            int newCol = current.col + dc;

            if (isValid(newRow, newCol, rows, cols) && maze[newRow][newCol] != 0) {
                int newDistance = current.distance + maze[newRow][newCol];
                if (newDistance < distances[newRow][newCol]) {
                    distances[newRow][newCol] = newDistance;
                    pq.push({newRow, newCol, newDistance});
                    predecessors[newRow][newCol] = {current.row, current.col, current.distance};
                }
            }
        }
    }

    std::vector<Point> path;
    Point current = end;

    while (current.row != -1 && current.col != -1) {
        path.push_back(current);
        current = predecessors[current.row][current.col];
    }

    if (path.empty() || path.back().row != start.row || path.back().col != start.col) {
        throw std::runtime_error("Path not found");
    }

    std::reverse(path.begin(), path.end());
    return path;
}

int main() {
    try {
        int rows, cols;

        std::cout << "Введите размеры лабиринта [n, m]: ";
        std::cin >> rows >> cols;

        if (rows <= 0 || cols <= 0) {
            throw std::invalid_argument("Размеры лабиринта должны быть положительными.\n");
        }

        std::vector<std::vector<int>> maze(rows, std::vector<int>(cols));
        std::cout << "Введите лабиринт (0 - препятствие, положительные числа (от 0 до 9) - вес клеток):\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cin >> maze[i][j];
                if (maze[i][j] > 9 || maze[i][j] < 0)
                    throw std::invalid_argument("Значение выходят за диапозон.\n");
            }
        }
        Point start, end;
        std::cout << "Введите координаты начальной точки [row, col]: ";
        std::cin >> start.row >> start.col;

        std::cout << "Введите координаты конечной точки [row, col]: ";
        std::cin >> end.row >> end.col;

        if (!isValid(start.row, start.col, rows, cols) || !isValid(end.row, end.col, rows, cols)) {
            throw std::invalid_argument("Координаты начальной или конечной точки выходят за пределы лабиринта.\n");
        }
        if (maze[start.row][start.col] == 0 || maze[end.row][end.col] == 0) {
            throw std::invalid_argument("Начальная или конечная точка находится в препятствии.\n");
        }

        std::vector<Point> path = findShortestPath(maze, start, end);

        std::cout << "Кратчайший путь:\n";
        for (const auto& point : path) {
            std::cout << point.row << " " << point.col << "\n";
        }
        std::cout << ".\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        exit(1);
    }

    exit(0);
}
