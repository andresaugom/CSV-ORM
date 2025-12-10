# CSV-ORM

**CSV-ORM** is a strongly-typed, modern C++17 library providing an ORM-like interface for treating CSV files as relational tables. It uses advanced template metaprogramming and member pointers to enable type-safe models, fluent query/mutation APIs, and an atomic, robust persistence layer—all with no external dependencies.

## Project Description

CSV-ORM enables developers to define C++ structs as table models, mapping CSV file columns directly to struct fields. It provides fluent, type-enforced APIs for querying, sorting, updating, and rolling back changes, much like a relational database ORM—but for local CSV storage. 

Strong type safety and modern C++ paradigms (including smart pointers and RAII) deliver a reliable, predictable developer experience, while in-memory operations and transaction-like editing grant fine-grained control for scripting, embedded apps, or quick data tasks.

**Key features:**
- Template-based model system, mapping CSV data to C++ structs.
- Type-strict, fluent query and mutation interface (`filter`, `sort`, `set`, `rollback`, etc).
- Atomic persistence layer with temporary file rewriting for data integrity.
- Rollback and transaction-style edits.
- Header-only, no external dependencies.
- C++17 required or newer.

---

## Project Structure

```bash
CSV-ORM/
├── include/
│   └── csvorm/
│       ├── Field.hpp             # Represents a single cell (nullable-aware)
│       ├── FieldInfo.hpp         # Metadata for fields (name and member pointer)
│       ├── ORM.hpp               # Entry point for full system
│       ├── parsing.hpp           # Parsing functions for string → type
│       ├── Table.hpp             # Core Table class (load/save/query/edit)
│       ├── TableQuery.hpp        # Provides fluent, filterable query interface
│       └── TableMutation.hpp     # Enables in-memory modification of table rows
│   └── csvorm.hpp                # Used to import ORM.hpp above
├── tests/
│   ├── data.csv                  # Sample test CSV
│   └── test_main.cpp             # Test runner with full usage
├── examples/
│   ├── data.csv                  # Example dataset
│   └── basic_usage.cpp           # Step-by-step usage demo
└── README.md
```

---

## Importing the library

**With CMake:**
```cmake
find_package(csvorm REQUIRED)
target_link_libraries(my_project PRIVATE csvorm)
```

Include in your code:
```cpp
#include <csvorm>
```

**Or manually:**

```bash
g++ -I/path/to/CSV-ORM/include my_file.cpp -o my_program
```

Include:
```cpp
#include <csvorm>
```

---

## Defining models

Define C++ struct models with `Field<T>` for each column:

```cpp
struct Person {
    Field<int> id;
    Field<std::string> name;
    Field<float> score;
};
```
Each `Field<T>` behaves like a smart value with:
- `.get()`    – Access value (throws if null)
- `.set(value)` – Set value
- `.is_null()`  – Check if value is null
- `.reset()`    – Clear value

---

## Mapping CSV Columns

Define a schema using `FieldInfo` that tells the ORM how to map CSV columns:

```cpp
std::vector<std::shared_ptr<FieldInfoBase>> schema;
schema.push_back(std::make_shared<FieldInfo<Person, Field<int>>>("id", &Person::id));
schema.push_back(std::make_shared<FieldInfo<Person, Field<std::string>>>("name", &Person::name));
schema.push_back(std::make_shared<FieldInfo<Person, Field<float>>>("score", &Person::score));
```

---

## Loading and Saving CSV Files

```cpp
Table<Person> table(schema, "data.csv", ',');

// Load CSV into memory as vector<Person>
table.load();

// Make changes (optional)
table.save();             // Overwrites original file
table.save("backup.csv"); // Saves to new file
```

---

## Querying Data

Queries are lazy and chainable, using `filter`, `sort`, `limit`, and `skip`.

```cpp
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

---

## Mutating Data

Mutations allow in-place editing of rows via `filter`, `set`, or `set_field`. Rollback is supported before applying, using `rollback` and `apply`.

```cpp
table.edit()
    .filter([](const Person& p) {
        return p.name.get() == "Andres";
    })
    .set_field(&Person::score, 10.0)
    .apply();   // Commits changes to memory
```

Or perform manual logic:

```cpp
table.edit()
    .set([](Person& p) {
        if (p.score.get() < 5.0) {
            p.name.set("LowScore");
        }
    })
    .apply();
```

---

## Rollback Support

Before calling `.apply()`, all changes can be undone:

```cpp
TableMutation<Person> edit = table.edit();
edit.set(...);
edit.rollback();  // Discards changes
```

---

## Features

- Template-based C++ model system
- Nullable-aware fields (`Field<T>`)
- CSV-to-object and object-to-CSV conversion
- In-memory table representation for efficient access
- Fluent-style filter/sort/limit/skip
- In-place mutation with rollback support
- Modern C++17 headers and idioms
- **Atomic** file operations for data safety

---

## Requirements

- C++17 or newer
- No external libraries (header-only)

---

## Future Improvements

- Support for writing headers
- Support for default values
- Pagination (lazy loading)

---

## Example

Check `examples/basic_usage.cpp` for a minimal working setup, and run `tests/test_main.cpp` for complete validation.

---

## License

GPLv3 – GNU GENERAL PUBLIC LICENSE Version 3.
