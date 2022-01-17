#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <vector>
#include <mytcpserver.h>
#include "mytcpsocket.h"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ExitBtn_triggered();

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void textChanged();
private:
    Ui::MainWindow *ui;
    vector<QLineEdit*> vec_line_edit;
    vector<QLabel*> vec_label;
    MyTcpServer* server;
    MyTcpSocket* socket;
};
#endif // MAINWINDOW_H
