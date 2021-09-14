/****************************************************************************************************
 *                                                                                                  *
 *                                                FFES                                              *
 *                                          by. Finn Eggers                                         *
 *                                                                                                  *
 *                    FFESis free software: you can redistribute it and/or modify                   *
 *                it under the terms of the GNU General Public License as published by              *
 *                 the Free Software Foundation, either version 3 of the License, or                *
 *                                (at your option) any later version.                               *
 *                       FFESis distributed in the hope that it will be useful,                     *
 *                   but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 *                   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 *                            GNU General Public License for more details.                          *
 *                 You should have received a copy of the GNU General Public License                *
 *                   along with FFES.  If not, see <http://www.gnu.org/licenses/>.                  *
 *                                                                                                  *
 ****************************************************************************************************/

#ifndef FEM_SRC_READER_READER_H_
#define FEM_SRC_READER_READER_H_

#include <fstream>
#include <ostream>
#include "LineType.h"
#include "LineData.h"

struct Reader {

    LineData line_data{};

    void read_NODE(const std::string& line){

    }
    void read_ELEMENT(const std::string& line){

    }
    void read_NONE(const std::string& line){

    }

    bool nextLine(std::ifstream& stream, std::string& line) {
        bool res = std::getline(stream, line).operator bool();
        if (!res)
            return false;

        line_data = line;
        while ((  line_data.line_type == COMMENT
               || line_data.line_type == EMPTY)
               && std::getline(stream, line)) {
            line_data = line;
        }

        if (line.empty())
            return false;


        return true;
    }

    void read(const std::string& file) {
        std::ifstream infile(file);

        auto fnc = &Reader::read_NONE;

        std::string   line;
        while (nextLine(infile, line)) {

            if(line_data.line_type == COMMENT) continue;
            if(line_data.line_type == EMPTY  ) continue;
            if(line_data.line_type == NORMAL ) (this->*fnc)(line_data.line);
            if(line_data.line_type == HEADER ){
                std::string header_name = line_data.commandName();
                if(header_name == "NODE"){
                    fnc = &Reader::read_NODE;
                }
                if(header_name == "ELEMENT"){
                    fnc = &Reader::read_ELEMENT;
                }
            }

            std::cout << line_data.line << std::endl;
        }
    }
};

#endif    // FEM_SRC_READER_READER_H_
