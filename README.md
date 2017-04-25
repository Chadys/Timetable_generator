***Presentation***
This program will create a possible timetable for several classes depending on time constraints of the teachers and classrooms.
The generated file will be a .xls.
Please be aware that this program takes a long time to execute (~10-15 minutes for 6 classes depending on your computer) and may fail to provide an answer if the time constraints are too limited.

***Requirements***
Boost library

***To compile and launch :***
$cmake CMakeLists.txt
$make
$Timetable_Generator name_file_classes_info name_file_teachers_info name_file_classrooms_info [name_generated_XSL] #default to Timetables.xls

***How to create the data files**
Please see example.data for all the formatting necessary