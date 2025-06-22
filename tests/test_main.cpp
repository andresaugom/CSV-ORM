// File: test_main.cpp

#include "../include/csvorm/Table.hpp"
#include <memory>

int main() {
    struct Person {
        Field<int> id;
        Field<std::string> name;
        Field<float> score;
    };

    std::string path = "./data.csv";
    std::vector<std::shared_ptr<FieldInfoBase>> structure;

    structure.push_back(std::make_shared<FieldInfo<Person, Field<int>>>("id", &Person::id));
    structure.push_back(std::make_shared<FieldInfo<Person, Field<std::string>>>("id", &Person::name));
    structure.push_back(std::make_shared<FieldInfo<Person, Field<float>>>("id", &Person::score));

    Table<Person> myTable(structure, path, ',');

    myTable.load();

    myTable.print_vector();
}