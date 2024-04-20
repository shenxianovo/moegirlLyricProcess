#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "extractor.h"

#include <QMainWindow>
#include <QPushButton>
#include <QListView>
#include <QFileDialog>
#include <QStringListModel>
#include <QVector>

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
    void openFile();
    void extract();

private:
    Ui::MainWindow *ui;
    QVector<Extractor> extractors;
};
#endif // MAINWINDOW_H
