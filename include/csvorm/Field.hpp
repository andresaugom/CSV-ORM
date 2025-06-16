// File: Field.hpp

#include <optional>
#include "Type.hpp"

template <typename T>
class Field {
    private:
        using value_type = T;
        std::optional<T> value;

    public:
        Field() = default;
        Field(const T& val) : value(val) {}    
    
        // Member functions
        const T& get() const { return this->value; }
        void set(const T& val)  { this->value = val; }
        const bool is_null() const { return this->value.has_value(); }
        void reset() { this->value.reset(); }
};