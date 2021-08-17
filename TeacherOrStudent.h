#ifndef TEACHERORSTUDENT_H
#define TEACHERORSTUDENT_H

#include <QWidget>
#include"Lecturer_LogIn_Form.h"
#include"Student_LogIn_Form.h"

namespace Ui {
class TeacherOrStudent;
}

class TeacherOrStudent : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherOrStudent(QWidget *parent = nullptr);
    ~TeacherOrStudent();


private slots:
    void on_pushButton_teacher_clicked();
    void on_pushButton_student_clicked();
    void on_pushButton_teacher_pressed();
    void on_pushButton_student_pressed();

private:
    Ui::TeacherOrStudent *ui;
    Lecturer_LogIn_Form *teacher;
    Student_LogIn_Form *student;
};

#endif // TEACHERORSTUDENT_H
