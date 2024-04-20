#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "extractor.h"

#include <QAction>
#include <QFileDialog>
#include <QstringList>
#include <QList>
#include <QVector>
#include <QStandardItem>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1200, 800);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->buttonExtract, &QPushButton::clicked, this, &MainWindow::extract);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Open File", "D:/codingArea/C_C++/C++_MoegirlLyricParse/HTMLVersion/HTML", "HTML Files (*.html *.htm)");

    // 将文件名传入Extractor中
    for (int i = 0; i < filePaths.size(); i++)
    {
        Extractor extractor = Extractor(filePaths.at(i));
        extractors.push_back(extractor);
    }

    // 将文件名显示在tableWidget中
    ui->tableWidget->setRowCount(filePaths.size());
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "File Name"
                                                             << "Chinese"
                                                             << "Japanese"
                                                             << "Kana Notation");

    // 设置整张表格不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for (int i = 0; i < filePaths.size(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(extractors[i].getFileName()));
    }

    // 在后三列设置按钮并居中
    for (int i = 0; i < filePaths.size(); i++)
    {
        QCheckBox *chineseButton = new QCheckBox();
        QCheckBox *japaneseButton = new QCheckBox();
        QCheckBox *kanaButton = new QCheckBox();

        ui->tableWidget->setCellWidget(i, 1, chineseButton);
        ui->tableWidget->setCellWidget(i, 2, japaneseButton);
        ui->tableWidget->setCellWidget(i, 3, kanaButton);

        // 设置按钮居中
        chineseButton->setStyleSheet("QCheckBox::center");
        japaneseButton->setStyleSheet("QCheckBox::center");
        kanaButton->setStyleSheet("QCheckBox::center");


        // 设置按钮不可选中
        if (!extractors[i].getKanaInfo())
        {
            kanaButton->setEnabled(false);
        }
        // 连接japaneseButton的stateChanged信号和一个自定义的槽函数
        connect(japaneseButton, &QCheckBox::stateChanged, [kanaButton](int state) {
            if (state == Qt::Unchecked && kanaButton->isEnabled() && kanaButton->isChecked()) { // 取消选择日语 同时取消选择假名
                kanaButton->setChecked(false);
            }
        });
        connect(kanaButton, &QCheckBox::stateChanged, [japaneseButton](int state) {
            if (state == Qt::Checked && !japaneseButton->isChecked()) { // 选择假名时同时选择日语
                japaneseButton->setChecked(true);
            }
        });
    }

    // 表格列宽调整
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::extract() {
    // 读取表格中内容与选择状态
    for (int i = 0; i < extractors.size(); i++)
    {
        extractors[i].selectionInfo[0] = extractors[i].selectionInfo[1] = extractors[i].selectionInfo[2] = 0;
        QCheckBox *chineseButton = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(i, 1));
        QCheckBox *japaneseButton = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(i, 2));
        QCheckBox *kanaButton = qobject_cast<QCheckBox *>(ui->tableWidget->cellWidget(i, 3));

        if (chineseButton->isChecked())
        {
            extractors[i].selectionInfo[0] = 1;
        }
        if (japaneseButton->isChecked())
        {
            extractors[i].selectionInfo[1] = 1;
        }
        if (kanaButton->isChecked())
        {
            extractors[i].selectionInfo[2] = 1;
        }

        // 提取歌词
        extractors[i].extract();
    }
    // 将提取后的歌词在textEdit中打开，显示在tabWidget中
    // 先删除原来的tab
    int tabCount = ui->tabWidget->count();
    for (int i = 0; i < tabCount; i++)
    {
        ui->tabWidget->removeTab(0);
    }
    for (int i = 0; i < extractors.size(); i++)
    {
        QString songName = extractors[i].getFileName().mid(0, extractors[i].getFileName().indexOf("-") - 1);
        QString savePath = "lyric/" + songName +".txt";
        QFile inputFile(savePath);
        QTextEdit *textEdit = new QTextEdit();

        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&inputFile);
            QString text = in.readAll();

            textEdit->setText(text);
        }

        ui->tabWidget->addTab(textEdit, songName);
    }
}
