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
    void loadDataIntoSource();
    void saveToFileFromTable();
    void saveToFileFromSource();
    void getProjects();
    void getDomains();
    void getGitBranches();
    QVector<LangData> getXliffData(QString lang);
    QString composeXliffHeader(QString lang);
    QString composeXliffBody(int row, int col);
    QString composeXliffFooter();

    const int TABLE_TAB_INDEX = 0;
    const int SOURCE_TAB_INDEX = 1;

    QString baseLang;
    QString currentLang;
    QString currentProject;
    QString currentDomain;
    QString currentGitBranch;
    QStringList projects;
    QStringList domains;
    QStringList gitBranches;

    ~MainWindow();

private slots:
    void on_quitAction_triggered();

    void on_gitPullButton_clicked();

    void on_gitCommitButton_clicked();

    void on_settingsAction_triggered();

    void on_translationsTabs_currentChanged(int index);

    void on_projectsListView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

