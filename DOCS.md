# CSV-ORM Documentation

## Overview

CSV-ORM is designed to provide a familiar ORM-like interface for manipulating CSV files as strongly typed relational tables in modern C++17. It uses templates and field metadata to enable safe, idiomatic data manipulation, transaction-like mutation, and robust persistence.

---

## Table of Contents

- [Defining Models](#defining-models)
- [Schema/Field Mapping](#schemafield-mapping)
- [Loading Data](#loading-data)
- [Querying](#querying)
- [Mutation and Transactions](#mutation-and-transactions)
- [Persistence Layer and Atomicity](#persistence-layer-and-atomicity)
- [Usage Example](#usage-example)
- [Limitations and TODOs](#limitations-and-todos)
- [License](#license)

---

## Defining Models

Models are plain C++ structs with each member wrapped in a `Field<T>`. This enables type-safe storage and nullable fields.

```cpp
struct Person {
    Field<int> id;
    Field<std::string> name;
    Field<float> score;
};
```
Each field instance can be checked for nullness, reset, set, or retrieved.

---

## Schema/Field Mapping

At runtime, you provide metadata for your model using a vector of `FieldInfo` instances (one per column):

```cpp
std::vector<std::shared_ptr<FieldInfoBase>> schema;
schema.push_back(std::make_shared<FieldInfo<Person, Field<int>>>("id", &Person::id));
schema.push_back(std::make_shared<FieldInfo<Person, Field<std::string>>>("name", &Person::name));
schema.push_back(std::make_shared<FieldInfo<Person, Field<float>>>("score", &Person::score));
```
This schema defines the CSV field order and enforces type conversion.

---

## Loading Data

Initialize a table instance with your schema and CSV path:

```cpp
Table<Person> table(schema, "data.csv", ',');
table.load(); // Loads all rows eagerly as vector<Person>
```

TODO:

- **Pagination support:** `load()` only supports full-file loads. Batch/paginated loading is mentioned in comments and planned.

---

## Querying

Querying is provided via a fluent, chainable API (`filter`, `sort`, `limit`, `skip`):

```cpp
auto adult_count = table.query()
    .filter([](const Person& p) { return p.age.get() >= 18; })
    .count();

auto best = table.query()
    .sort([](const Person& a, const Person& b) { return a.score.get() > b.score.get(); })
    .limit(1)
    .first();
```

All queries are type-checked at compile time and never rely on runtime type juggling or casts.

---

## Mutation and Transactions

The mutation API exposes type-checked, in-place row modification with rollback (undo):

```cpp
table.edit()
    .filter([](const Person& p) { return p.name.get() == "Andres"; })
    .set_field(&Person::score, 42.0)
    .apply();   // Commits changes

// Manual update
table.edit()
    .set([](Person& p) { p.score.set(p.score.get() + 10); })
    .apply();

// Rollback usage
auto ed = table.edit();
ed.set(...);
ed.rollback(); // All changes discarded
```

- Changes are kept in memory until `.apply()` is called.
- Rollback (`.rollback()`) is always available until you commit.

---

## Persistence Layer and Atomicity

CSV-ORM’s persistence layer is atomic by design:

- All changes are flushed using temporary file rewriting: on `save()`, the new file is written separately and then moved into place to prevent partial/corrupt state on crash.
- RAII patterns guarantee resource cleanup.

TODO:

- **Add explicit transactional boundaries for multi-table situations.**
- **File locking is not implemented.** Concurrent edits across processes are not protected.
- **Header/column name writing:** Currently, headers are not written; only data rows are persisted.
- **Default field values:** Not yet present in the schema logic.

---

## Usage Example

```cpp
// Define your model
struct Person {
    Field<int> id;
    Field<std::string> name;
    Field<float> score;
};

std::vector<std::shared_ptr<FieldInfoBase>> schema;
schema.push_back(std::make_shared<FieldInfo<Person, Field<int>>>("id", &Person::id));
schema.push_back(std::make_shared<FieldInfo<Person, Field<std::string>>>("name", &Person::name));
schema.push_back(std::make_shared<FieldInfo<Person, Field<float>>>("score", &Person::score));

Table<Person> people(schema, "data.csv", ',');
people.load();

auto top5 = people.query()
    .sort([](const Person& a, const Person& b) { return a.score.get() > b.score.get(); })
    .limit(5)
    .to_vector();

for (const auto& p : top5) {
    std::cout << p.name.get() << ": " << p.score.get() << std::endl;
}
```
See `examples/basic_usage.cpp` and `tests/test_main.cpp` for more.

---

## Limitations and TODOs

**IMPORTANT — These features are planned/needed soon:**
- [ ] Support for writing CSV headers (column names)
- [ ] Default field values in model and schema
- [ ] Batch/paginated loading (`Table::load()` supports only eager loading)
- [ ] File locking and cross-process concurrency protection
- [ ] More granular transactional control for multi-table or partial updates
- [ ] Richer error reporting (current errors are mostly exceptions on I/O or schema mis-match)
- [ ] Extended documentation (API reference, more complex examples)
- [ ] More file format support (currently only supports `,` as separator, but can be parameterized)

---

## License

GPLv3 — see LICENSE.
