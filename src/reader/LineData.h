

#ifndef FEM_SRC_READER_LINEDATA_H_
#define FEM_SRC_READER_LINEDATA_H_

#include "LineType.h"

struct LineData {

    std::string line;
    std::string csv[32];

    int         csv_count = 0;
    LineType    line_type = COMMENT;

    LineData&   operator  =(const std::string& p_line) {
        this->csv_count = 0;
        this->line      = p_line;

        this->line.erase(std::remove_if(this->line.begin(), this->line.end(), isspace),
                         this->line.end());

//        for (int i = 0; i < (int) this->line.length(); i++) {
//            this->line[i] = std::toupper(this->line[i]);
//        }

        auto it = std::begin(this->line);
        while (true) {
            auto commaPosition = std::find(it, std::end(this->line), ',');
            csv[csv_count++]   = std::string(it, commaPosition);
            if (commaPosition == std::end(this->line)) {
                break;
            }
            it = std::next(commaPosition);
        }

        if (line.empty()) {
            line_type = EMPTY;
        }
        if (line.rfind("**", 0) == 0) {
            line_type = COMMENT;
        } else if (line.rfind("*", 0) == 0) {
            line_type = HEADER;
        } else {
            line_type = NORMAL;
        }

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const LineData& data) {

        os << "line     : " << data.line << "\n";
        os << "csv [" << std::setw(3) << data.csv_count << "]:\n";
        for (int i = 0; i < data.csv_count; i++) {
            os << "          " << std::left << std::setw(40) << data.csv[i] << "\n";
        }
        return os;
    }

    std::string requireValue(const std::string& key) {
        for (int i = 0; i < csv_count; i++) {
            if (csv[i].rfind(key, 0) == 0) {    // pos=0 limits the search to the prefix

                // read value
                // 1) check if "=" is contained
                ERROR(csv[i].find('=') < csv[i].length(), PARSING_SYNTAX_ERROR, csv[i])

                // 2) split at "=" and get second value
                std::string value = csv[i].substr(csv[i].find('=') + 1, csv[i].length());
                return value;
            }
            ERROR(i < csv_count - 1, PARSING_SYNTAX_ERROR, key << ":" << line);
        }
        return "";
    }

    std::string parseValue(const std::string& key, const std::string& def) {
        for (int i = 0; i < csv_count; i++) {
            if (csv[i].rfind(key, 0) == 0) {    // pos=0 limits the search to the prefix

                // read value
                // 1) check if "=" is contained
                ERROR(csv[i].find('=') < csv[i].length(), PARSING_SYNTAX_ERROR, csv[i])

                // 2) split at "=" and get second value
                std::string value = csv[i].substr(csv[i].find('=') + 1, csv[i].length());
                return value;
            }
        }
        return def;
    }

    std::string commandName() { return csv->substr(1, csv->length()); }
};

#endif    // FEM_SRC_READER_LINEDATA_H_
