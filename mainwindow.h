#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "langdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void loadDataIntoTable();
    void saveToFile(QString lang);
    QString composeXliffHeader(QString lang);
    QString composeXliffBody(int row, int col);
    QString composeXliffFooter();
    QVector<LangData> getXliffData(QString lang);
    QString baseLang;
    QString currentLang;
    ~MainWindow();

private slots:
    void on_quitAction_triggered();

    void on_gitPullButton_clicked();

    void on_gitCommitButton_clicked();

    void on_settingsAction_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

