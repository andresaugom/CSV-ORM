// File: Table.hpp

#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "FieldInfo.hpp"
#include "Query.hpp"

template <typename T>
class Table {
    private:
        char def_separator = ',';
        std::vector<T> rows;
        std::vector<FieldInfoBase*> schema;
        std::string path; // Path to file .csv

    public:
        Table(
            const std::vector<FieldInfoBase*>& schema, 
            const std::string& path, 
            char def_separator
        ) : schema(schema), path(path), def_separator(def_separator) {

        }

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

        void save() {

        }

        void insert(T new_row) {
            rows.push_back(new_row);
        }

        void delete_row(int row_index) {
            if (row_index >= 0 && row_index < rows.size()) {
                rows.erase(rows.begin() + row_index);
            }
        }



        TableQuery<T> query() const {
            return TableQuery<T>(this->rows);
        }

        ~Table() = default;
};