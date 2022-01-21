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
struct Barrier
{
    float x;
    float y;
    float width;
    float height;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:

private slots:
    void on_ExitBtn_triggered();

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void textChanged(const QString &arg1);
    void textChangedBarrier1(const QString &arg1);
    void textChangedBarrier2(const QString &arg1);
    void textChangedBarrier3(const QString &arg1);
    void textChangedBarrier4(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

private:
    int ind;
    Ui::MainWindow *ui;
    vector<QLineEdit*> vec_line_edit;
    vector<QLabel*> vec_label;
    MyTcpServer* server;
    MyTcpSocket* socket;
    vector<Barrier> vec_barrier;
};
#endif // MAINWINDOW_H
