// File: TableMutation
#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>

template <typename T>
class TableMutation { 
    private:
        std::vector<T*> rows_to_mutate;
        std::vector<T> rollback_cache;

    public:
        TableMutation(std::vector<T>& input_rows) {
            for (T& r : input_rows) {
                rows_to_mutate.push_back(&r);
                rollback_cache.push_back(r);
            }
        }
    
        // Apply a filter using function. This function MUST be boolean
        TableMutation<T>& filter(std::function<bool(const T&)> predicate) {
            std::vector<T*> result;
            std::vector<T> new_rollback;

            for (size_t i = 0; i < rows_to_mutate.size(); ++i) {
                if (predicate(*rows_to_mutate[i])) {
                    result.push_back(rows_to_mutate[i]);
                    new_rollback.push_back(rollback_cache[i]);
                }
            }
            this->rows_to_mutate = std::move(result);
            this->rollback_cache = std::move(new_rollback);

            return *this;
        }

        // Set values as specified on the function
        TableMutation<T>& set(std::function<void(T&)> setter) {
            for (T* r : rows_to_mutate) {
                setter(*r);
            }
            return *this;
        }

        /** 
         * Set a value on a column. Use 'filter()' to 
         * apply only on some columns.
         */ 
        template <typename FieldType>
        TableMutation<T>& set_field(FieldType T::* member, const typename FieldType::value_type& value) {
            for (T* r : rows_to_mutate) {
                (r->*member).set(value);
            }
            return *this;
        }

        /**
         * Set a value on a column based on a function. 
         * Use 'filter()' to apply only on some columns.
         */
        template<typename FieldType>
        TableMutation<T>& set_field(FieldType T::* member, std::function<typename FieldType::value_type(const T&)> value_fn) {
            for (T* r : rows_to_mutate) {
                (r->*member).set(value_fn(*r));
            }
            return *this;
        }

        // Returns the 'rows_to_mutate' as it was on 'rollback_cache'
        void rollback() {
            for (size_t i = 0; i < rows_to_mutate.size(); ++i) {
                *rows_to_mutate[i] = rollback_cache[i];
            }
        }

        // Cannot do rollback after apply
        TableMutation<T>& apply() {
            rollback_cache = rows_to_mutate;
            return *this;
        }

        /**
         * Apply a function for each item found matching the criteria, if 
         * 'filter()' was used. Instead, for each of all items.
         */
        void for_each(std::function<void(T&)> action) {
            for (T* r : rows_to_mutate) {
                action(*r);
            }
        }

        /** 
         * Returns the number of items matching the criteria, if 
         * 'filter()' was used. Instead, all items on csv.
         */
        int count() {
            return static_cast<int>(rows_to_mutate.size());
        }
};