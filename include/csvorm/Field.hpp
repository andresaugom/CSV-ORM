// File: Field.hpp

#include <optional>

/**
 * #Field
 * Represents a cell on the table
 * 
 * ##Description
 * Supported fields types are: 
 * int, float, double, string, bool
 * 
 * ##Additional notes:
 * 
 */
template <typename T>
class Field {
    private:
        std::optional<T> value;

    public:
        using value_type = T;

        Field() = default;
        Field(const T& val) : value(val) {}    
    
        // Member functions
        const T& get() const {
            if (is_null()) throw std::runtime_error("Trying to access null Field value");
            return this->value.value();
        }
        void set(const T& val)  { this->value = val; }
        bool is_null() const { return !this->value.has_value(); }
        void reset() { this->value.reset(); }

        friend std::ostream& operator<<(std::ostream& os, const Field<T>& f) {
            if (!f.is_null()) {
                os << f.get();
            } 
            else {
                os << "";
            }
            return os;
        }

        ~Field() = default;
};