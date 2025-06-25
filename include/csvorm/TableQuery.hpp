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
        
        // Apply a filter using function. This function MUST be boolean
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

        // Sort the vector as specified on the boolean function
        TableQuery<T>& sort(std::function<bool(const T&)> comparator) {
            std::sort(filtered_rows.begin(), filtered_rows.end(),
                [&](const T* a, const T* b) {
                    return comparator(*a, *b);
                });
            return *this;
        }

        // Limit the obtained result on query
        TableQuery<T>& limit(int n) {
            if (n < filtered_rows.size()) {
                filtered_rows.resize(n);
            }
            return *this;
        }

        // Skips the first 'n' results
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

        /** 
         * Export the filtered data as a vector of type 'T'
         * being 'T' the struct defined by user.
         */
        std::vector<T> to_vector() {
            std::vector<T> result;
            for (const T* r : filtered_rows) {
                result.push_back(*r);
            }
            return result;
        }

        /** 
         * Export the filtered data as a vector of type 'const T*',
         * being 'T' the struct defined by user.
         */
        std::vector<const T*> to_pointers() {
            return filtered_rows;
        }

        /**
         * Apply a function for each item found matching the criteria, if 
         * 'filter()' was used. Instead, for each of all items.
         */
        void for_each(std::function<void(const T&)> action) {
            for (const auto& r : filtered_rows) {
                action(*r);
            }
        }

        // Returns the first result that matches the criteria
        T first() {
            if (filtered_rows.empty()) {
                throw std::runtime_error("No rows match the query. ");
            }
            else {
                return *filtered_rows.front();
            }
        }

        // Returns the number of elements matching the criteria
        int count() {
            return static_cast<int>(filtered_rows.size());
        }

        /** Boolean result. 'True' means there is at least 1 element matching the
         * criteria. 'False' means there is no elements matching the criteria
         */
        bool exists() {
            return !filtered_rows.empty();
        }

};  