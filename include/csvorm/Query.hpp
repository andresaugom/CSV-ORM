// File: Query.hpp

#pragma once

template <typename T>
class TableQuery {
private:
    const std::vector<T>* original_data;
    std::vector<const T*> filtered_view;

public:
    TableQuery(const std::vector<T>& data)
        : original_data(&data) {
        for (const auto& row : data) {
            filtered_view.push_back(&row);
        }
    }

    TableQuery& where(auto predicate) {
        std::vector<const T*> new_view;
        for (auto* row : filtered_view) {
            if (predicate(*row)) {
                new_view.push_back(row);
            }
        }
        filtered_view = std::move(new_view);
        return *this;
    }

    TableQuery& sort_by(auto comparator) {
        std::sort(filtered_view.begin(), filtered_view.end(),
                  [&](const T* a, const T* b) {
                      return comparator(*a, *b);
                  });
        return *this;
    }

    void print() const {
        for (auto* row : filtered_view) {
            // Mostrar cada fila como sea necesario
        }
    }

    // Export as a vector
    std::vector<const T*> collect() const {
        return filtered_view;
    }
};

