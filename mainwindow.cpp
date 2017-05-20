#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "langdata.h"
#include "settingsdialog.h"
#include <QDomDocument>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    baseLang = "ru";
    currentLang = "en";
    ui->setupUi(this);

    ui->menuView->addAction(ui->gitBranchesDock->toggleViewAction());
    ui->menuView->addAction(ui->domainsDock->toggleViewAction());
    ui->menuView->addAction(ui->projectsDock->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(ui->appLogsDock->toggleViewAction());
    ui->appLogsDock->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quitAction_triggered()
{
    QApplication::quit();
}

void MainWindow::on_gitPullButton_clicked()
{
    loadDataIntoTable();
}

void MainWindow::loadDataIntoTable()
{
    QStringList horizontalHeaders;
    QStringList verticalHeaders;

    QVector<LangData> baseLangData    = getXliffData(baseLang);
    QVector<LangData> currentLangData = getXliffData(currentLang);

    ui->translationsTable->setRowCount(baseLangData.length());
    ui->translationsTable->setColumnCount(2);

    for(int i = 0; i < baseLangData.length(); i++)
    {
        verticalHeaders << baseLangData[i].key;

        QTableWidgetItem *baseLangItem = new QTableWidgetItem();
        baseLangItem->setText(baseLangData[i].value);
        baseLangItem->setFlags(Qt::ItemIsEditable);
        ui->translationsTable->setItem(i, 0, baseLangItem);

        for(int j = 0; j < currentLangData.length(); j++)
        {
            if (currentLangData[j].key == baseLangData[i].key)
            {
                QTableWidgetItem *currentLangItem = new QTableWidgetItem();
                currentLangItem->setText(QString::number(currentLangData[j].id) + ": " + currentLangData[j].value);
                ui->translationsTable->setItem(i, 1, currentLangItem);
            }
        }
    }

    horizontalHeaders << baseLang << currentLang;

    ui->translationsTable->setVerticalHeaderLabels(verticalHeaders);
    ui->translationsTable->setHorizontalHeaderLabels(horizontalHeaders);
    ui->translationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QVector<LangData> MainWindow::getXliffData(QString lang)
{
    QVector<LangData> xliffData;
    QDomDocument doc("global." + lang + ".xliff");

    QFile file("global." + lang + ".xliff");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Не могу открыть файл";
        return xliffData;
    }

    if (!doc.setContent(&file)) {
         qDebug() << "Не пропарсить файл";
        file.close();
        return xliffData;
    }

    QTextStream ReadFile(&file);
    ui->translationsSourceTextEdit->setText(ReadFile.readAll());

    file.close();

    QDomElement root = doc.documentElement();
    QDomNode fileNode = root.firstChild();
    QDomNode bodyNode = fileNode.firstChild();
    QDomNode transNode = bodyNode.firstChild();

    while(!transNode.isNull()) {
        QDomElement transElement = transNode.toElement();

        if(!transElement.isNull()) {        
            QDomAttr attribute = transElement.attributeNode("id");
            QString idStr = attribute.value();

            QDomNode sourceNode = transNode.firstChild();
            QDomNode targetNode = sourceNode.nextSibling();

            QDomElement sourceElement = sourceNode.toElement();
            QDomElement targetElement = targetNode.toElement();

            xliffData.push_back(LangData(sourceElement.text(), targetElement.text(), idStr.toInt()));
        }
        transNode = transNode.nextSibling();
    }

    return xliffData;
}

void MainWindow::on_gitCommitButton_clicked()
{
    saveToFile(currentLang);
}

void MainWindow::saveToFile(QString lang)
{
    QFile file("global." + lang + ".xliff");

    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&file);

        out.setCodec("UTF-8");
        out << composeXliffHeader(lang);

        int count = ui->translationsTable->rowCount();
        for (int row = 0 ; row < count ; ++row) {
            out << composeXliffBody(row, 1);
        }

        out << composeXliffFooter();
        out.flush();
    }

    file.close();
}

QString MainWindow::composeXliffHeader(QString lang) {
    return QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n") +
        "<xliff xmlns=\"urn:oasis:names:tc:xliff:document:1.2\" version=\"1.2\">\n" +
        "  <file source-language=\"" + lang + "\" datatype=\"plaintext\" original=\"file.ext\">\n" +
        "    <body>\n";
}

QString MainWindow::composeXliffBody(int row, int col) {
    return QString("      ") + "<trans-unit id=\"" + QString::number(row + 1) + "\">\n" +
        "        <source>" + ui->translationsTable->verticalHeaderItem(row)->text() + "</source>\n" +
        "        <target>" + ui->translationsTable->item(row, col)->text() + "</target>\n" +
        "      </trans-unit>\n";
}

QString MainWindow::composeXliffFooter() {
    return QString("    </body>\n") +
        "  </file>\n" +
        "</xliff>\n";
}

void MainWindow::on_settingsAction_triggered()
{
    SettingsDialog settingsDialog;
    int res = settingsDialog.exec();

    if(res == 1)
    {
        currentLang = settingsDialog.currentLang;
    }
}
