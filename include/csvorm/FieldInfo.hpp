// File: FieldInfo.hpp

#include "parsing.hpp"
#include <string>
#include <sstream>

/**
 * #FieldInfo
 * Works as metadata for each 'Field' on table
 * 
 * ##Description:
 * This class allows storing the name of the column
 * and the pointer to the struct member defined by user.
 */
struct FieldInfoBase {
    std::string name;

    virtual void set_from_string(void* instance, const std::string& value) = 0;
    virtual std::string get_as_string(void* instance) = 0;
    virtual ~FieldInfoBase() = default;
};

template<typename T, typename FieldType>
struct FieldInfo : public FieldInfoBase {
    FieldType T::* member_ptr;

    // Call it like "FieldInfo("first_column", &CSV_struct::first_column)"
    FieldInfo(const std::string& name, FieldType T::* member_ptr)
    : member_ptr(member_ptr) {
        this-> name = name;
    }

    void set_from_string(void* instance, const std::string& value) {
        T* obj = static_cast<T*>(instance);
        obj->*member_ptr = parse_value<typename FieldType::value_type>(value);
    }

    std::string get_as_string(void* instance) {
        T* obj = static_cast<T*>(instance);
        std::ostringstream oss;
        oss << obj->*member_ptr;
        return oss.str();
    }
};
