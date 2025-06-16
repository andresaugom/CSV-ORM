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
        int n = schema.size(), m = rows.size(); // 'n' stands for number of columns, m for rows.
        char def_separator = ',';
        std::vector<T> rows;
        std::vector<FieldInfoBase*> schema;
        std::string path; // Path to file .csv

    public:
        Table(const std::vector<FieldInfoBase*>& schema, const std::string& path, char def_separator) : schema(schema), path(path), def_separator(def_separator) {

        }

        void load() {
            std::ifstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error ("Cannot open file: " + path);
            }

            std::vector<std::string> csv_lines;
            std::string line;
            
            while(std::getline(file, line)) csv_lines.push_back(line);

            for (const std::string& line : csv_lines) {
                
            }
        }

        void save() {

        }

        void insert(T new_row) {
            rows.push_back(new_row);
        }

        void delete_row(int row_index) {
            rows.erase(row_index);
        }



        TableQuery<T> query() const {
            return TableQuery<T>(this->rows);
        }

        ~Table() = default;
};