// File: TableQuery.hpp

#pragma once
#include <functional>
#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>
#include <stdexcept>

template <typename T>
class TableQuery {
    private:
        const std::vector<T>& original_rows;
        std::vector<const T*> filtered_rows;

    public:
        TableQuery(const std::vector<T>& table_rows) : original_rows(table_rows) {
            for (const T& r : table_rows) {
                filtered_rows.push_back(&r);
            }
        }
        
        TableQuery<T>& filter(std::function<bool(const T&)> predicate) {
            std::vector<const T*> result;
            for (const T* r : filtered_rows) {
                if (predicate(*r)) {
                    result.push_back(r);
                }
            }
            filtered_rows = std::move(result);
            return *this;
        }

        TableQuery<T>& sort(std::function<bool(const T&)> comparator) {
            std::sort(filtered_rows.begin(), filtered_rows.end(),
                [&](const T* a, const T* b) {
                    return comparator(*a, *b);
                });
            return *this;
        }

        TableQuery<T>& limit(int n) {
            if (n < filtered_rows.size()) {
                filtered_rows.resize(n);
            }
            return *this;
        }

        TableQuery<T>& skip(int n) {
            if (n >= filtered_rows.size()) {
                filtered_rows.clear();
            }
            else {
                filtered_rows = std::vector<const T*>(
                    filtered_rows.begin() + n,
                    filtered_rows.end()
                );
            }
            return *this;
        }

        std::vector<T> to_vector() {
            std::vector<T> result;
            for (const T* r : filtered_rows) {
                result.push_back(*r);
            }
            return result;
        }

        std::vector<const T*> to_pointers() {
            return filtered_rows;
        }

        void for_each(std::function<void(const T&)> action) {
            for (const auto& r : filtered_rows) {
                action(*r);
            }
        }

        T first() {
            if (filtered_rows.empty()) {
                throw std::runtime_error("No rows match the query. ");
            }
            else {
                return *filtered_rows.front();
            }
        }

        int count() {
            return static_cast<int>(filtered_rows.size());
        }

        bool exists() {
            return !filtered_rows.empty();
        }

};  