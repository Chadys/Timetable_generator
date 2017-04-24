#include <iostream>
#include <map>
#include <functional>
#include "DataProvider.h"
#include "NRPA.h"
#include "Timetable.h"

int main(int argc, char* argv[]) {
    if (argc != 4){
        std::cout << "Usage: Timetable_Generator [Filename for classes infos]" << ' ' <<
                "[Filename for teachers infos]" << ' ' << "[Filename for classrooms infos]" << std::endl;
        return 1;
    }

    DataProvider provider;
    provider.init(argv[3], argv[1], argv[2]);
    NRPA solver(provider);
    vector<Timetable> timetables = solver.generate();
    if(!timetables.empty()) {
        Timetable::create_excel(timetables, provider);
        std::cout << "Failed to create valid timetables." << std::endl;
        return 1;
    }
    std::cout << "Timetables.xls created !" << std::endl;
    GraphFonc::print_graph(solver.get_graph());
    return 0;
}