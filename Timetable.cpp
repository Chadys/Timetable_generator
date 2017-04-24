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

    //Bold&HCenter&Vcenter
    ExcelFormat::XLSFormatManager fmt_mgr(e);
    ExcelFormat::ExcelFont font_bold;
    font_bold._weight = 700;
    ExcelFormat::CellFormat fmt_title(fmt_mgr);
    fmt_title.set_font(font_bold);
    fmt_title.set_alignment(ExcelFormat::EXCEL_HALIGN_CENTRED | ExcelFormat::EXCEL_VALIGN_CENTRED);
    //HCenter
    ExcelFormat::CellFormat fmt_courses(fmt_mgr);
    fmt_courses.set_alignment(ExcelFormat::EXCEL_HALIGN_CENTRED);
    //VTop
    ExcelFormat::CellFormat fmt_hours(fmt_mgr);
    fmt_hours.set_alignment(ExcelFormat::EXCEL_VALIGN_TOP);

    for (unsigned int i = 0; i < timetables.size(); ++i) {
        sheet = e.GetWorksheet(i);
        if(sheet){
            // Title : Class' name
            cell = sheet->Cell(0, 0);
            cell->Set(static_cast<string>(*timetables[i].students).c_str());
            cell->SetFormat(fmt_title);
            sheet->MergeCells(0,0,2,Time::days.size()+1); //double row size for each row == merge two rows
            // Days as columns
            for(auto &day : Time::days) {
                sheet->Cell(4, day.first + 1)->Set(day.second.c_str());
                sheet->SetColWidth(day.first+1, 10000);
                sheet->MergeCells(4, day.first + 1, 2, 1);
            }
            // Hours as rows
            for (unsigned int j = 0; j < provider.all_times.front().size(); ++j) {
                cell = sheet->Cell(j*2 + 6, 0);
                cell->Set(provider.all_times.front()[j].hour);
                sheet->MergeCells(j*2 + 6, 0, 2, 1);
                cell->SetFormat(fmt_hours);
            }
            // Insert each period
            for (const auto &period : timetables[i].periods){
                unsigned int row = period.first.hour*2+6, col = period.first.day+1;
                cell = sheet->Cell(row, col);
                cell->Set((period.second.course->title+"\n"+period.second.teacher->name).c_str());
                cell->SetFormat(fmt_courses);
                sheet->MergeCells(row, col, 2*(period.second.course->hours_number/period.second.course->type), 1);
            }
            e.RenameWorksheet(i, static_cast<string>(*timetables[i].students).c_str());
        }
    }
    e.SaveAs("Timetables.xls");
}