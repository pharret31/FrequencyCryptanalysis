#ifndef FREQUENCIESTABLEMODEL_H
#define FREQUENCIESTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QPair>
#include <QColor>

class FrequenciesTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    FrequenciesTableModel();
    ~FrequenciesTableModel();

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent);

    void addRecord(QPair<QString, float> data);
    void clear();
    void setSendSignal(bool sendSignal);

private:
    QVector<QPair<QString, float> > m_data;

    bool m_sendSignal;

signals:
    void oldDataChanged(QString from, QString to);
};

#endif // FREQUENCIESTABLEMODEL_H
