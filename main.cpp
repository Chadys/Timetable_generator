#include <iostream>
#include <map>
#include <functional>
#include "DataProvider.h"
#include "NRPA.h"
#include "Timetable.h"

int main(int argc, char* argv[]) {
    if (argc < 4){
        std::cout << "Usage: Timetable_Generator [Filename for classes infos]" << ' ' <<
                "[Filename for teachers infos]" << ' ' << "[Filename for classrooms infos]" <<
                "[Filename for produced .xls (Optional)]" << std::endl;
        return 1;
    }
    const char *excel_name = (argc == 4) ? "Timetables.xls" : argv[4];

    DataProvider provider;
    provider.init(argv[3], argv[1], argv[2]);
    NRPA solver(provider);
    vector<Timetable> timetables = solver.generate();
    GraphFonc::print_graph(solver.get_graph(), provider);
    if(timetables.empty()) {
        std::cout << "Failed to create valid timetables." << std::endl;
        return 1;
    }
    Timetable::create_excel(timetables, provider, excel_name);
    std::cout << excel_name << " created !" << std::endl;
    return 0;
}