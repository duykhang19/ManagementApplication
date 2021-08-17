#include "TeacherOrStudent.h"
#include "ui_TeacherOrStudent.h"

TeacherOrStudent::TeacherOrStudent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherOrStudent)
{
    ui->setupUi(this);
    teacher = new Lecturer_LogIn_Form();
    student = new Student_LogIn_Form();
    ui->pushButton_student->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_teacher->setFocusPolicy(Qt::NoFocus);
}

TeacherOrStudent::~TeacherOrStudent()
{
    delete ui;
    delete teacher;
    delete student;
}



void TeacherOrStudent::on_pushButton_teacher_clicked()
{
    teacher->show();
    close();
}

void TeacherOrStudent::on_pushButton_student_clicked()
{
    student->show();
    close();
}

void TeacherOrStudent::on_pushButton_teacher_pressed()
{
    teacher->show();
    close();
}

void TeacherOrStudent::on_pushButton_student_pressed()
{
    student->show();
    close();
}
