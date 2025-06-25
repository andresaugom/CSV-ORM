// File: test_main.cpp

#include "../include/csvorm/Table.hpp"
#include <memory>

int main(int argc, char* argv[]) {
    bool restore_data = false;

    if (argc > 1 && std::string(argv[1]) == "--restore") {
        restore_data = true;
    }
    
    struct Person {
        Field<int> id;
        Field<std::string> name;
        Field<float> score;
    };

    if (restore_data) {
        std::string path = "../examples/data.csv";
        std::vector<std::shared_ptr<FieldInfoBase>> structure;
        std::vector<std::shared_ptr<Person>> data;

        structure.push_back(std::make_shared<FieldInfo<Person, Field<int>>>("id", &Person::id));
        structure.push_back(std::make_shared<FieldInfo<Person, Field<std::string>>>("id", &Person::name));
        structure.push_back(std::make_shared<FieldInfo<Person, Field<float>>>("id", &Person::score));

        Table<Person> myTable(structure, path, ',');

        myTable.load();

        myTable.save("data.csv");

        return 0;
    }

    std::string path = "data.csv";
    std::vector<std::shared_ptr<FieldInfoBase>> structure;

    structure.push_back(std::make_shared<FieldInfo<Person, Field<int>>>("id", &Person::id));
    structure.push_back(std::make_shared<FieldInfo<Person, Field<std::string>>>("id", &Person::name));
    structure.push_back(std::make_shared<FieldInfo<Person, Field<float>>>("id", &Person::score));

    Table<Person> myTable(structure, path, ',');

    myTable.load();

    std::cout << "Showing if person named 'Andres' exists: ";
    std::cout << myTable.query().filter(
    [](const Person& p) {
        return p.name.get() == "Andres";
    }
    ).exists() << std::endl;

    std::cout << "\nCount how much users with id < 3 exists: ";
    std::cout << myTable.query().filter(
        [](const Person& p) {
            return p.id.get() < 3;
        }
    ).count() << std::endl;

    std::cout << "All users: " << std::endl;
    myTable.print_vector();


    // Using set_field method
    std::cout << "Changing Andres's user ID to '10': " << std::endl;
    myTable.edit().filter(
        [](const Person& p) {
            return p.name.get() == "Andres";
        }
    ).set_field(&Person::id, 10).apply();

    // Using set method
    std::cout << "Changing some fields with specified parameters: " << std::endl;
    myTable.edit().set(
        [](Person& p) {
            if (p.name.get() == "Andres") {
                p.name.set("Andres2");
            }
            if (p.score.get() < 8.0) {
                p.score.set(10.0);
            }
            if (p.id.get()%2 == 0) {
                p.id.set(p.id.get()*3);
            }
        }
    ).apply();

    std::cout << "Showing changes: " << std::endl;
    myTable.print_vector();

    std::cout << "Saving on file." << std::endl;
    myTable.save();
    std::cout << "File saved succesfully." << std::endl;

    return 0;
}
