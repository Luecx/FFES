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

#include "../system/System.h"
#include "LineData.h"
#include "LineType.h"

#include <fstream>
#include <ostream>

class Reader {

    LineData      line_data {};
    std::ifstream infile;

    std::string   file;
    Reader*       parent            = nullptr;
    System*       system            = nullptr;

    int           max_node_count    = 0;
    int           max_element_count = 0;

    public:
    Reader(const std::string& p_file, Reader* p_parent = nullptr) {
        file   = p_file;
        parent = p_parent;
    }

    private:
    void openFile() {
        infile = std::ifstream {file};
        ERROR(infile.is_open(), CANNOT_OPEN_FILE, file)
    }
    void closeFile() { infile.close(); }
    void nextLine() {
        bool res = std::getline(infile, line_data.line).operator bool();
        if (!res) {
            line_data.line_type = END_OF_FILE;
            return;
        }

        line_data = line_data.line;
        while ((line_data.line_type == COMMENT || line_data.line_type == EMPTY)
               && std::getline(infile, line_data.line)) {
            line_data = line_data.line;
        }

        if (line_data.line.empty())
            line_data.line_type = END_OF_FILE;
    }
    System* getSystem() {
        if (parent != nullptr) {
            return parent->getSystem();
        }
        return system;
    }

    void read_NODE() {
        auto        sys       = getSystem();
        std::string nset_name = this->line_data.requireValue("NSET");
        sys->model.activateNodeSet(nset_name);
        nextLine();
        while (line_data.line_type != END_OF_FILE && line_data.line_type != HEADER) {

            if (line_data.line_type == COMMENT)
                continue;

            auto id = std::stoi(line_data.csv[0]) - 1;

            auto x  = std::stof(line_data.csv[1]);
            auto y  = std::stof(line_data.csv[2]);
            auto z  = line_data.csv_count > 3 ? std::stof(line_data.csv[3]) : 0;

            sys->model.setNode(id, x, y, z);

            nextLine();
        }
    }
    void read_NSET() {
        auto        sys       = getSystem();
        std::string nset_name = this->line_data.requireValue("NSET");
        sys->model.activateNodeSet(nset_name);
        nextLine();
        while (line_data.line_type != END_OF_FILE && line_data.line_type != HEADER) {

            if (line_data.line_type == COMMENT)
                continue;

            for (int h = 0; h < line_data.csv_count; h++) {
                if (!line_data.csv[h].empty())
                    sys->model.addNodeToNodeSet(nset_name, std::stoi(line_data.csv[h]) - 1);
            }

            nextLine();
        }
    }
    void read_ELEMENT() {
        auto        sys        = getSystem();
        std::string elset_name = this->line_data.requireValue("ELSET");
        std::string type       = this->line_data.requireValue("TYPE");
        sys->model.activateElementSet(elset_name);
        nextLine();

        while (line_data.line_type != END_OF_FILE && line_data.line_type != HEADER) {

            if (line_data.line_type == COMMENT)
                continue;

            auto id = std::stoi(line_data.csv[0]) - 1;

            if (type == "C3D8") {
                auto n1 = std::stoi(line_data.csv[1]) - 1;
                auto n2 = std::stoi(line_data.csv[2]) - 1;
                auto n3 = std::stoi(line_data.csv[3]) - 1;
                auto n4 = std::stoi(line_data.csv[4]) - 1;
                auto n5 = std::stoi(line_data.csv[5]) - 1;
                auto n6 = std::stoi(line_data.csv[6]) - 1;
                auto n7 = std::stoi(line_data.csv[7]) - 1;
                auto n8 = std::stoi(line_data.csv[8]) - 1;
                sys->model.setElement<Iso3DHex8>(id, n1, n2, n3, n4, n5, n6, n7, n8);
            } else {
                ERROR(false, PARSING_SYNTAX_ERROR, "element type not known: " << type);
            }
            nextLine();
        }
    }
    void read_INCLUDE() {
        auto   file_name = line_data.requireValue("INPUT");
        Reader reader {file_name, this};
        reader.read();
        nextLine();
    }
    void read_MATERIAL() {
        std::string mat_name = this->line_data.requireValue("NAME");
        auto        sys      = getSystem();
        ID          id       = sys->model.addMaterial(mat_name);
        nextLine();

        while (line_data.line_type != END_OF_FILE) {
            if (line_data.line_type == COMMENT) {

            } else if (line_data.line_type == EMPTY) {

            } else if (line_data.line_type == NORMAL) {

            } else if (line_data.line_type == HEADER) {
                std::string header_name = line_data.commandName();
                if (header_name == "ELASTIC") {
                    std::string type = line_data.requireValue("TYPE");
                    nextLine();
                    if (type == "ISOTROPIC") {
                        Precision young   = std::stod(line_data.csv[0]);
                        Precision poisson = std::stod(line_data.csv[1]);
                        sys->model.materials[id]->setElasticity<IsotropicElasticity>(young, poisson);
                    } else {
                        ERROR(false, ELEMENT_TYPE_NOT_KNOWN, type);
                    }
                    nextLine();
                } else if (header_name == "DENSITY") {
                    nextLine();
                    Precision density                 = std::stod(line_data.csv[0]);
                    sys->model.materials[id]->density = density;
                } else {
                    break;
                }
            }
            nextLine();
        }
    }
    void read_SOLIDSECTION() {
        nextLine();

        std::string element_set   = line_data.csv[0];
        std::string material_name = line_data.csv[1];
        getSystem()->model.solidSection(element_set, material_name);
        nextLine();
    }
    void read_LOADCASE() {
        getSystem()->addLoadCase<LoadCase>();
        nextLine();
    }
    void read_BOUNDARY() {
        bool is_temporary = line_data.parseValue("OP", "") == "TEMPORARY";
        nextLine();
        while (line_data.line_type != END_OF_FILE && line_data.line_type != HEADER) {
            if (line_data.line_type == COMMENT)
                continue;
            if (line_data.csv_count < 3)
                continue;
            auto dim = std::stoi(line_data.csv[1]) - 1;
            auto dis = std::stof(line_data.csv[2]);
            if (getSystem()->model.getNodeSetID(line_data.csv[0]) >= 0) {
                getSystem()->getLoadCase()->constraint(line_data.csv[0], dim, dis, is_temporary);
            } else {
                getSystem()->getLoadCase()->constraint(std::stoi(line_data.csv[0]) - 1, dim, dis,
                                                       is_temporary);
            }
            nextLine();
        }
    }
    void read_CLOAD() {
        bool is_temporary = line_data.parseValue("OP", "") == "TEMPORARY";
        nextLine();
        while (line_data.line_type != END_OF_FILE && line_data.line_type != HEADER) {
            if (line_data.line_type == COMMENT)
                continue;
            if (line_data.csv_count < 3)
                continue;
            auto dim = std::stoi(line_data.csv[1]) - 1;
            auto dis = std::stof(line_data.csv[2]);
            if (getSystem()->model.getNodeSetID(line_data.csv[0]) >= 0) {
                getSystem()->getLoadCase()->applyLoad(line_data.csv[0], dim, dis, is_temporary);
            } else {
                getSystem()->getLoadCase()->applyLoad(std::stoi(line_data.csv[0]) - 1, dim, dis,
                                                      is_temporary);
            }
            nextLine();
        }
    }

    void allocate() {

        openFile();
        line_data.line_type   = NORMAL;

        bool reading_nodes    = false;
        bool reading_elements = false;

        while (line_data.line_type != END_OF_FILE) {

            if (line_data.line_type == COMMENT) {

            } else if (line_data.line_type == EMPTY) {

            } else if (line_data.line_type == NORMAL) {
                if (reading_nodes) {
                    max_node_count = std::max(max_node_count, std::stoi(line_data.csv[0]));
                }
                if (reading_elements) {
                    max_element_count = std::max(max_element_count, std::stoi(line_data.csv[0]));
                }
            } else if (line_data.line_type == HEADER) {
                std::string header_name = line_data.commandName();
                if (header_name == "NODE") {
                    reading_nodes    = true;
                    reading_elements = false;
                } else if (header_name == "ELEMENT") {
                    reading_nodes    = false;
                    reading_elements = true;
                } else {
                    reading_nodes    = false;
                    reading_elements = false;
                }

                if (header_name == "INCLUDE") {
                    auto   file_name = line_data.requireValue("INPUT");
                    Reader reader {file_name, this};
                    reader.openFile();
                    reader.allocate();
                    reader.closeFile();
                    max_node_count += reader.max_node_count;
                    max_element_count += reader.max_element_count;
                }
            }
            this->nextLine();
        }
        closeFile();
    }

    public:
    System* read() {
        if (parent == nullptr) {
            allocate();
            system = new System(max_node_count, max_element_count);
        }
        line_data = LineData {};

        openFile();

        while (line_data.line_type != END_OF_FILE) {
            std::cout << line_data.line << std::endl;
            if (line_data.line_type == COMMENT)
                nextLine();
            else if (line_data.line_type == EMPTY)
                nextLine();
            else if (line_data.line_type == NORMAL)
                nextLine();
            else if (line_data.line_type == HEADER) {
                std::string header_name = line_data.commandName();
                if (header_name == "NODE") {
                    read_NODE();
                } else if (header_name == "NSET") {
                    read_NSET();
                } else if (header_name == "ELEMENT") {
                    read_ELEMENT();
                } else if (header_name == "INCLUDE") {
                    read_INCLUDE();
                } else if (header_name == "MATERIAL") {
                    read_MATERIAL();
                } else if (header_name == "SOLIDSECTION") {
                    read_SOLIDSECTION();
                } else if (header_name == "LOADCASE") {
                    read_LOADCASE();
                } else if (header_name == "BOUNDARY") {
                    read_BOUNDARY();
                } else if (header_name == "CLOAD") {
                    read_CLOAD();
                } else {
                    nextLine();
                    WARNING(false, "Command not known: " << header_name);
                }
            }
        }
        std::cout << "EOF" << std::endl;
        closeFile();
        return system;
    }
};

#endif    // FEM_SRC_READER_READER_H_
