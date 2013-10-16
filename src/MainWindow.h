#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Plots;
class SetterView;
class File;
class Algorithm;

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
    void load();
    void go();
    void setTitle();
    void wasFinished(Algorithm* algorithm);
private:
    Ui::MainWindow *ui;
    SetterView* setterView;
    Plots* plots;
    File* file;
};

#endif // MAINWINDOW_H
