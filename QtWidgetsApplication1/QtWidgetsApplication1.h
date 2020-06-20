#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include <QTimer>

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtWidgetsApplication1Class ui;
    QTimer* timer;
    QImage qt_image;

private slots:
    void on_pushButton_open_cam_clicked();

    void on_pushButton_close_cam_clicked();

    void update_window();
};
