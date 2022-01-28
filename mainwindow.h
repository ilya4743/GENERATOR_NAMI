#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <vector>
#include <QCheckBox>
#include <mytcpserver.h>
#include "mytcpsocket.h"
#include"point.h"

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
class MyTcpSocket;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void data_received(QDataStream &stream);
private slots:
    void data_send(QDataStream &stream);

    void on_ExitBtn_triggered();
    void on_lineEdit_textChanged(const QString &arg1);

    void textChanged(const QString &arg1);
    void textChanged1(const QString &s);
    void textChangedBarrier1(const QString &arg1);
    void textChangedBarrier2(const QString &arg1);
    void textChangedBarrier3(const QString &arg1);
    void textChangedBarrier4(const QString &arg1);

    void currentIndexCenterChanged(int index);
    void on_comboBox_currentIndexChanged(int index);


    void on_action_triggered();
    void motionSimulation(int state);
    void loopSimulation(int state);
    void endlessSimulation(int state);
    void on_sendDataBtn_clicked();

private:
    int ind;
    vector<QLineEdit*> vec_line_edit;
    vector<QLabel*> vec_label;
    MyTcpServer* server;
    MyTcpSocket* socket;

    QComboBox qComboBox;
public:    Ui::MainWindow *ui;
    vector<Barrier> vec_barrier;
    vector<Barrier> vec_buf_barrier;
    Point goal_point;
    Point goal_point_buf;
    int count_line;
    void recalculateBarrier(Point P);
    void recalculateGoal(Point p);
    void recalculateGoalX(float x);
    void makePack();    vector<QCheckBox*> vec_check_box;


};
#endif // MAINWINDOW_H
