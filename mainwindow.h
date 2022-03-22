#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <vector>
#include "mytcpserver.h"
#include "mytcpsocket.h"
#include "point.h"
#include "Barrier.h"
#include "GameMap.h"
#include "Car.h"
#include "XMLGenerator.h"

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

    void on_ExitBtn_triggered();

    void textChanged(const QString &arg1);
    void textChanged1(const QString &s);
    void textChangedBarrier1(const QString &arg1);
    void textChangedBarrier2(const QString &arg1);
    void textChangedBarrier3(const QString &arg1);
    void textChangedBarrier4(const QString &arg1);

    void currentIndexCenterChanged(int index);
    void on_comboBox_currentIndexChanged(int index);


    void motionSimulation(int state);
    void loopSimulation(int state);
    void endlessSimulation(int state);
    void waitBPR(int state);
    void on_sendDataBtn_clicked();
    void stopSimulationBtn_click();
    void on_SettingsBtn_triggered();
    void on_importSeq_triggered();
    void on_exportSeq_triggered();

private:
    int ind;
    vector<QLineEdit*> vec_line_edit;
    vector<QLabel*> vec_label;
    MyTcpServer* server;
    MyTcpSocket* socket;
    QComboBox qComboBox;
    vector<QCheckBox*> vec_check_box;
    QSpinBox* spinBoxN;
    void updateWidgets();
    void updateValues();
public:
    QPushButton *stop_btn;
    Ui::MainWindow *ui;
    vector<Barrier> vec_barrier;
    vector<Barrier> vec_buf_barrier;
    Point goal_point;
    Point goal_point_buf;
    int count_line;
    int count_line_buf;
    int countSendPack;
    int countReceivePack;
    void recalculateBarrier(Point P);
    void recalculateGoal(Point p);
    void recalculateGoalX(float x);
    void makePack();
    bool isMotionSimulation;
    bool isLoopSimulation;
    bool isEndlessSimulation;
    bool isWaitBPR;
    bool stopSimulation;
    Car car;
    GameMap map;
    int countBarriers;
};
#endif // MAINWINDOW_H
