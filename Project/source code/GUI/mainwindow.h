#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QtWidgets>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "vesseldetector.h"
#include "utils.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_pushButton_process_clicked();

    void on_actionDRIVE_Params_triggered();

    void on_actionSTARE_Params_triggered();

    void on_actionCHASE_Params_triggered();

    void on_pushButton_accuracy_clicked();

    void on_actionLoad_Default_Params_triggered();

    void on_pushButton_opticdisc_clicked();

private:
    Ui::MainWindow *ui;
    VesselDetector vdetector;
    void initApp();
    void setParams();
    void display(const cv::Mat& , QLabel * , bool );
    void grow(const cv::Mat& image, cv::Mat dst, int x, int y);
};

#endif // MAINWINDOW_H
