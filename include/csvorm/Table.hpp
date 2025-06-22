// File: Table.hpp

#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "Field.hpp"
#include "FieldInfo.hpp"
#include "TableQuery.hpp"
#include "TableMutation.hpp"

/**
 * You define a struct like this:
 * 
 *  struct MyCSV {
 *      Field<int> id;
 *      Field<std::string> name;
 *      Field<float> anotherField;
 *      ...
 *  };
 * 
 * and then, a FieldInfo vector which will tell the ORM 
 * how to read the CSV and call every column:
 * 
 *  std::vector<FieldInfo*> schema;
 * 
 *  schema.push_back(FieldInfo<MyCSV, int>("MyID", MyCSV::id*));
 *  schema.push_back(FieldInfo<MyCSV, std::string>("MyName", MyCSV::name*));
 *  schema.push_back(FieldInfo<MyCSV, float>("MyAnotherField", MyCSV::anotherField*));
 *  ...
 * 
 */

// Type T MUST be the struct defined by user.
template <typename T>
class Table {
    private:
        char def_separator = ',';
        std::vector<T> rows;
        std::vector<std::shared_ptr<FieldInfoBase>> schema;
        std::string path; // Path to file .csv

    public:
        Table(
            const std::vector<std::shared_ptr<FieldInfoBase>>& schema, 
            const std::string& path, 
            char def_separator
        ) : schema(schema), path(path), def_separator(def_separator) {}

        /**
         * # Get all CSV objects loaded on ram.
         * 
         * ## Pending to implement:
         * - Loading objects by batches / pagination
         */
        void load() {
            std::ifstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error ("Cannot open file: " + path);
            }

            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string cell;
                std::vector<std::string> tokens;

                while (std::getline(ss, cell, def_separator)) {
                    tokens.push_back(cell);
                }

                if (tokens.size() != schema.size()) {
                    throw std::runtime_error("CSV row does not match schema size.");
                }

                T row_instance;
                for (size_t i = 0; i < schema.size(); ++i) {
                    schema[i]->set_from_string(&row_instance, tokens[i]);
                }

                rows.push_back(std::move(row_instance));
            }
        }

        /**
         * # Write all objects on 'rows' attribute. 
         * 
         * ### Details:
         * - This method writes AS SORTED ON VECTOR. If you want to
         * write the objects on any order, use 'sort(...)' method
         * - The sort method is not implemented on TableQuery yet.
         */
        void save() {
            std::stringstream result;

            for (const T& row : rows) {
                for (size_t i = 0; i < schema.size(); ++i){ 
                    result << schema[i]->get_as_string((void*)&row);

                    if (i != schema.size() - 1) {
                        result << def_separator;
                    }
                }
                result << "\n";
            }

            std::ofstream file(path);
            file << result.str(); 
        }

        void insert(T new_row) {
            rows.push_back(new_row);
        }

        void delete_row(int row_index) {
            if (row_index >= 0 && row_index < rows.size()) {
                rows.erase(rows.begin() + row_index);
            }
        }

        // This function is just to test / debug
        void print_vector() {
            std::stringstream result;

            for (const T& row : rows) {
                for (size_t i = 0; i < schema.size(); ++i){ 
                    result << schema[i]->get_as_string((void*)&row);

                    if (i != schema.size() - 1) {
                        result << def_separator;
                    }
                }
                result << "\n";
            }
            std::cout << result.str();
        }

        TableQuery<T> query() const {
            return TableQuery<T>(this->rows);
        }

        TableMutation<T> edit() {
            return TableMutation<T>(this->rows);
        }

        ~Table() = default;
};