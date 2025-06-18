// File: FieldInfo.hpp

#include "Field.hpp"
#include "parsing.hpp"

#include <string>
#include <sstream>

struct FieldInfoBase {
    std::string name;
    Type type;

    virtual void set_from_string(void* instance, const std::string& value) = 0;
    virtual ~FieldInfoBase() = default;
};

template<typename T, typename FieldType>
struct FieldInfo : public FieldInfoBase {
    FieldType T::* member_ptr;

    // Call it like "FieldInfo("first_column", Type::INT, &CSV_struct::first_column)"
    FieldInfo(const std::string& name, const Type& type, FieldType T::* member_ptr)
    : member_ptr(member_ptr) {
        this->name = name;
        this->type = type;
    }

    void set_from_string(void* instance, const std::string& value) {
        T* obj = static_cast<T*>(instance);
        obj->*member_ptr = parse_value<typename FieldType::value_type>(value);
    }
};
