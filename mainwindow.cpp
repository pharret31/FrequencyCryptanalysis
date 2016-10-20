#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    m_codeTextEdit = new QTextEdit();

    QPushButton *analyzeButton = new QPushButton(tr("Analyze"));
    connect(analyzeButton, SIGNAL(clicked()),
            this, SLOT(onAnalyzeButtonClick()));

    m_frequencyTable = new QTableView();
    m_frequencyTableModel = new FrequenciesTableModel();
    connect(m_frequencyTableModel, SIGNAL(oldDataChanged(QString,QString)),
            this, SLOT(onLetterChanged(QString,QString)));
    m_frequencyTable->setModel(m_frequencyTableModel);
    m_frequencyTable->verticalHeader()->hide();
    m_frequencyTable->horizontalHeader()->hide();
    m_frequencyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_frequencyTable->setFixedHeight(76);

    m_encodeTextEdit = new QTextEdit();

    QHBoxLayout *editLayout = new QHBoxLayout();

    QLabel *editLine = new QLabel(tr("Change "));

    m_beforeLine = new QLineEdit();

    QLabel *onLine = new QLabel(tr(" with "));

    m_afterLine = new QLineEdit();

    QPushButton *changeButton = new QPushButton(tr("Change"));
    connect(changeButton, SIGNAL(clicked()),
            this, SLOT(onChangeButtonClick()));

    editLayout->addWidget(editLine);
    editLayout->addWidget(m_beforeLine);
    editLayout->addWidget(onLine);
    editLayout->addWidget(m_afterLine);
    editLayout->addWidget(changeButton);

    QPushButton *digramAnalyzeButton = new QPushButton(tr("Digram analyze"));
    connect(digramAnalyzeButton, SIGNAL(clicked()),
            this, SLOT(onDigramAnalyzeButtonClick()));

    mainLayout->addWidget(m_codeTextEdit);
    mainLayout->addWidget(analyzeButton);
    mainLayout->addWidget(m_frequencyTable);
    mainLayout->addWidget(m_encodeTextEdit);
    mainLayout->addLayout(editLayout);
    mainLayout->addWidget(digramAnalyzeButton);

    this->setLayout(mainLayout);
    this->setFixedSize(500, 400);
    this->setWindowTitle("Cryptanalyst assistant");
}

MainWindow::~MainWindow()
{
    delete m_afterLine;
    delete m_beforeLine;
    delete m_encodeTextEdit;
    delete m_frequencyTable;
    delete m_frequencyTableModel;
    delete m_codeTextEdit;
}

bool MainWindow::sortLettersDistribution(const QPair<QString, float> &p1, const QPair<QString, float> &p2)
{
    return p1.second > p2.second;
}

void MainWindow::onAnalyzeButtonClick()
{
    m_frequencyTableModel->setSendSignal(false);

    QMap<QString, int> letters;
    letters.clear();
    QString cryptoText = m_codeTextEdit->toPlainText();
    m_encodeText = cryptoText;
    m_encodeTextEdit->setText(m_encodeText);
    int i = 0;
    int sumOfSymbols = 0;
    while(i < cryptoText.size()) {
        if (cryptoText[i].isNumber() && cryptoText[i + 1].isNumber()) {
            // Increment letter count in map
            QString let = cryptoText.at(i);
            let = let + cryptoText.at(i + 1);
            letters[let]++;
            i+=2;
            sumOfSymbols++;
            continue;
        }
        i++;
    }

    QVector<QPair<QString, float> > lettersVectorDistribution;
    for (QMap<QString, int>::const_iterator i = letters.begin(); i != letters.end(); i++) {
        lettersVectorDistribution.push_back(QPair<QString, float>(i.key(), i.value()*1.0 / sumOfSymbols));
    }

    qSort(lettersVectorDistribution.begin(), lettersVectorDistribution.end(), sortLettersDistribution);

    m_frequencyTableModel->clear();
    for (QVector<QPair<QString, float> >::const_iterator i = lettersVectorDistribution.begin(); i != lettersVectorDistribution.end(); i++) {
        m_frequencyTable->model()->insertColumn(m_frequencyTable->model()->columnCount(QModelIndex()));

        QModelIndex index = m_frequencyTable->model()->index(0, m_frequencyTable->model()->columnCount() - 1);
        m_frequencyTable->model()->setData(index, i->first);
        index = m_frequencyTable->model()->index(1, m_frequencyTable->model()->columnCount() - 1);
        m_frequencyTable->model()->setData(index, i->second);
    }

    m_frequencyTableModel->setSendSignal(true);
}

void MainWindow::onChangeButtonClick()
{
    if (m_beforeLine->text().isEmpty() || m_afterLine->text().isEmpty()) {
        QMessageBox::warning(this, tr("Field is empty"), tr("Some field is empty"), QMessageBox::Ok);
        return;
    }

    QString strFirst = m_beforeLine->text();
    QString strSecond = m_afterLine->text();
    onLetterChanged(m_beforeLine->text(), "~");
    onLetterChanged(m_afterLine->text(), strFirst);
    onLetterChanged("~", strSecond);
}

void MainWindow::onDigramAnalyzeButtonClick()
{
}

void MainWindow::onLetterChanged(QString from, QString to)
{
    int i = 0;
    // Flag uses to change two-digit letters to a one-symbol letter
    if (!m_encodeText.endsWith(" ")) {
        m_encodeText.append(" ");
    }
    bool flag = m_encodeText.indexOf(QRegExp("^\\d")) % 2 == 0;
    if (!from[0].isDigit()) {
        m_encodeText.replace(from, to);
    }
    else if (from[0].isDigit() && from.size() == 2) {
        while (i < m_encodeText.size() - 1) {
            if (m_encodeText[i].isNumber() && m_encodeText[i + 1].isNumber()) {
                if (i % 2 == 0) {
                    if (!flag) {
                        i++;
                        continue;
                    }
                }
                else {
                    if (flag) {
                        i++;
                        continue;
                    }
                }
                QString temp = m_encodeText.at(i);
                temp += m_encodeText.at(i + 1);
                if (temp == from) {
                    m_encodeText.replace(i, 2, to);
                    flag = !flag;
                }
            }
            else if (!m_encodeText[i].isNumber() && i != 0) {
                flag = !flag;
            }
            i++;
        }
    }
    m_encodeTextEdit->setText(m_encodeText);
}
