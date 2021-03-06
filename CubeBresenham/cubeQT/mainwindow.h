#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "dglwidget.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void resizeEvent(QResizeEvent *e) override;

private:
    std::unique_ptr <Ui::MainWindow> ui;
    dglWidget *wid = nullptr;
};

#endif // MAINWINDOW_H
