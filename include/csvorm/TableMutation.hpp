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

        TableMutation<T>& set(std::function<void(T&)> setter) {
            for (T* r : rows_to_mutate) {
                setter(*r);
            }
            return *this;
        }

        template <typename FieldType>
        TableMutation<T>& set_field(FieldType T::* member, const typename FieldType::value_type& value) {
            for (T* r : rows_to_mutate) {
                (r->*member).set(value);
            }
            return *this;
        }

        template<typename FieldType>
        TableMutation<T>& set_field(FieldType T::* member, std::function<typename FieldType::value_type(const T&)> value_fn) {
            for (T* r : rows_to_mutate) {
                (r->*member).set(value_fn(*r));
            }
            return *this;
        }

        void rollback() {
            for (size_t i = 0; i < rows_to_mutate.size(); ++i) {
                *rows_to_mutate[i] = rollback_cache[i];
            }
        }

        TableMutation<T>& apply() {
            return *this;
        }

        void for_each(std::function<void(T&)> action) {
            for (T* r : rows_to_mutate) {
                action(*r);
            }
        }

        int count() {
            return static_cast<int>(rows_to_mutate.size());
        }
};