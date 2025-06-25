# CSV-ORM

**CSV-ORM** is a lightweight ORM-like library written in modern C++ for working with CSV files as if they were relational tables. It uses templates and pointers to members to simulate model-based access, query, and mutation—without relying on any external libraries.

## Proyect Structure:

```bash
CSV-ORM/
├── include/
│   └── csvorm/
│       ├── Field.hpp             # Represents a single cell (nullable-aware)
│       ├── FieldInfo.hpp         # Metadata for fields (name and member pointer)
│       ├── ORM.hpp               # (Planned) Entry point for full system
│       ├── parsing.hpp           # Parsing functions for string → type
│       ├── Table.hpp             # Core Table class (load/save/query/edit)
│       ├── TableQuery.hpp        # Provides fluent, filterable query interface
│       └── TableMutation.hpp     # Enables in-memory modification of table rows
├── tests/
│   ├── data.csv                  # Sample test CSV
│   └── test_main.cpp             # Test runner with full usage
├── examples/
│   ├── data.csv                  # Example dataset
│   └── basic_usage.cpp           # Step-by-step usage demo
└── README.md
```

## Importing the library

If you are using CMake:

```cmake
find_package(csvorm REQUIRED)
target_link_libraries(my_project PRIVATE csvorm)
```

And then you can do:

```c++
#include <csvorm>
```

Or if you want to add it to the path:

```bash
g++ -I/path/to/CSV-ORM/include my_file.cpp -o my_program
```

And then you can do:

```c++
#include <csvorm>
```

## Defining models

Models must inherit from the Model base class (or be treated as such) and define fields using Field<T>, which supports nullability, validation, and conversion.

```c++
struct Person {
    Field<int> id;
    Field<std::string> name;
    Field<float> score;
};
```
Each `Field<T>` behaves like a smart value with:
- `.get()` – Access value (throws if null)
- `.set(value)` – Set value
- `.is_null()` – Check if value is null
- `.reset()` – Clear value

## Mapping CSV columns

You define a schema for your model using `FieldInfo` objects, which tell the ORM how to map each CSV column:

```c++
std::vector<std::shared_ptr<FieldInfoBase>> schema;

schema.push_back(std::make_shared<FieldInfo<Person, Field<int>>>("id", &Person::id));
schema.push_back(std::make_shared<FieldInfo<Person, Field<std::string>>>("name", &Person::name));
schema.push_back(std::make_shared<FieldInfo<Person, Field<float>>>("score", &Person::score));
```

## Loading and Saving CSV Files

```c++
Table<Person> table(schema, "data.csv", ',');

// Load CSV into memory as vector<Person>
table.load();

// Make changes (optional)
table.save();               // Overwrites original file
table.save("backup.csv");   // Saves to new file
```

## Querying Data

Queries are lazy and chainable, using `filter`, `sort`, `limit`, and `skip`.

```c++
int count = table.query()
    .filter([](const Person& p) {
        return p.score.get() >= 8.0;
    })
    .count();

std::vector<Person> top_users = table.query()
    .sort([](const Person& a, const Person& b) {
        return a.score.get() > b.score.get();
    })
    .limit(5)
    .to_vector();
```

## Mutating Data

Mutations allow in-place editing of rows via `filter`, `set`, or `set_field`. They support rollback using `rollback` before applying, using `apply`.

```c++
table.edit()
    .filter([](const Person& p) {
        return p.name.get() == "Andres";
    })
    .set_field(&Person::score, 10.0)
    .apply();   // Commits changes to memory
```

Or perform manual logic:

```c++
table.edit()
    .set([](Person& p) {
        if (p.score.get() < 5.0) {
            p.name.set("LowScore");
        }
    })
    .apply();
```

## Rollback Support

Before calling .apply(), all changes can be undone via:

```c++
TableMutation<Person> edit = table.edit();
edit.set(...);
edit.rollback();  // Discards changes
```

## Features

- Template-based model system
- Nullable-aware fields (Field<T>)
- CSV-to-object conversion
- In-memory table representation
- Fluent-style filtering, sorting, slicing
- In-place mutation with rollback

## Requirements

- C++17 or newer
- No external libraries (header-only)

## Future Improvements

- Support for writing headers
- Support for default values
- Pagination (lazy loading)

## Example

Check `examples/basic_usage.cpp` for a minimal working setup and run `test_main.cpp` for full validation.

## License

GNU GENERAL PUBLIC LICENSE Version 3.