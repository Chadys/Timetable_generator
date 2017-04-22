//
// Created by julie on 18/04/17.
//

#include <locale>
#include <codecvt>
#include "Timetable.h"


Timetable::Timetable(std::shared_ptr<Students> students_) : students(students_) {}

Timetable::Period::Period(std::shared_ptr<Course> course_, std::shared_ptr<Teacher> teacher_) : course(course_), teacher(teacher_) {}


vector<Timetable> Timetable::get_timetables_from_graph(Graph &graph){
    std::map<string, Timetable> timetables;
    typename boost::graph_traits<Graph>::vertex_iterator it, it_end;
    for (boost::tie(it, it_end) = boost::vertices(graph) ; it != it_end ; it++ ) {
        if(timetables.find(static_cast<string>(*graph[*it].students)) == timetables.end())
            timetables.emplace(*graph[*it].students, Timetable(graph[*it].students));
        for (int i = 0; i < graph[*it].course->type; ++i)
            timetables[*graph[*it].students].periods[graph[*it].time[i*2]] = {graph[*it].course, graph[*it].teacher};
    }
    vector<Timetable> timetables_vec;
    std::transform(timetables.begin(), timetables.end(),
                   std::back_inserter(timetables_vec), [](auto &kv){ return kv.second;});
    return timetables_vec;
}

int Timetable::evaluate(vector<Timetable> tables, DataProvider &provider){
    int score = 0;
    unsigned short used_days = 0;
    bool one_course = true;
    unsigned short first_hour = provider.all_times.front().front().hour;
    for (const Timetable &t : tables){
        TimeAccessor last_time;
        Period last_period;
        for (auto &kv : t.periods){
            if(last_time){
                if (last_time.day != kv.first.day) {
                    //if only one course in a day
                    if(one_course)
                        score-=10;
                    one_course = true;
                    used_days++;
                }
                else{
                    one_course = false;
                    //if free period
                    if(kv.first.hour - last_time.hour > 1){
                        //for lunch time
                        if(last_time.hour+first_hour < 12 || kv.first.hour+first_hour > 13) {
                            if (kv.first.hour + first_hour - last_time.hour + first_hour < 2)
                                score += 10;
                        }
                        //for nothing
                        else
                            score-=3*(kv.first.hour - last_time.hour);
                    }
                    //TODO : in else, lower score if classrooms !=
                }
            }
            last_time = kv.first;
            last_period = kv.second;
        }
        // bonus for every free day;
        score += (4-used_days)*5;
        used_days = 0;
    }
    return score;
}

//TODO : add classroom managment using Time.courses_number

void Timetable::create_excel(vector<Timetable> timetables, DataProvider &provider){
    ExcelFormat::BasicExcel e;
    ExcelFormat::BasicExcelWorksheet* sheet;
    ExcelFormat::BasicExcelCell* cell;

    e.New(timetables.size());

    //Bold&Center
    ExcelFormat::XLSFormatManager fmt_mgr(e);
    ExcelFormat::ExcelFont font_bold;
    font_bold._weight = 700;
    ExcelFormat::CellFormat fmt_bold(fmt_mgr);
    fmt_bold.set_font(font_bold);
    fmt_bold.set_alignment(ExcelFormat::EXCEL_HALIGN_CENTRED);
    //Center
    ExcelFormat::CellFormat fmt_center(fmt_mgr);
    fmt_center.set_alignment(ExcelFormat::EXCEL_HALIGN_CENTRED);

    for (unsigned int i = 0; i < timetables.size(); ++i) {
        sheet = e.GetWorksheet(i);
        if(sheet){
            // Title : Class' name
            cell = sheet->Cell(0, 0);
            cell->Set(static_cast<string>(*timetables[i].students).c_str());
            cell->SetFormat(fmt_bold);
            sheet->MergeCells(0,0,1,Time::days.size()+1);
            // Days as columns
            for(auto &day : Time::days) {
                sheet->Cell(2, day.first + 1)->Set(day.second.c_str());
                sheet->SetColWidth(day.first+1, 10000);
            }
            // Hours as rows
            for (unsigned int j = 0; j < provider.all_times.front().size(); ++j)
                sheet->Cell(j+3, 0)->Set(provider.all_times.front()[j].hour);
            // Insert each period
            for (const auto &period : timetables[i].periods){
                unsigned int row = period.first.hour+3, col = period.first.day+1;
                cell = sheet->Cell(row, col);
                cell->Set((period.second.course->title+"\n"+period.second.teacher->name).c_str());
                cell->SetFormat(fmt_center);
                sheet->MergeCells(row, col, period.second.course->hours_number/period.second.course->type, 1);
            }
            e.RenameWorksheet(i, static_cast<string>(*timetables[i].students).c_str());
        }
    }
    e.SaveAs("Timetables.xls");
}