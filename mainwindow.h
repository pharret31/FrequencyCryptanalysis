#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>

#include <QTextEdit>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>

#include "frequenciestablemodel.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QTextEdit *m_codeTextEdit;

    QTableView *m_frequencyTable;
    FrequenciesTableModel *m_frequencyTableModel;

    QTextEdit *m_encodeTextEdit;
    QString m_encodeText;

    QLineEdit *m_beforeLine;
    QLineEdit *m_afterLine;

    static bool sortLettersDistribution(const QPair<QString, float> &p1, const QPair<QString, float> &p2);

signals:

public slots:
    void onAnalyzeButtonClick();
    void onChangeButtonClick();
    void onDigramAnalyzeButtonClick();

    void onLetterChanged(QString from, QString to);
};

#endif // MAINWINDOW_H
