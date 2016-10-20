#include "frequenciestablemodel.h"

FrequenciesTableModel::FrequenciesTableModel()
{
    m_sendSignal = false;
    m_data.clear();
}

FrequenciesTableModel::~FrequenciesTableModel()
{
    m_data.clear();
}

int FrequenciesTableModel::columnCount(const QModelIndex &) const
{
    return m_data.count();
}

int FrequenciesTableModel::rowCount(const QModelIndex &) const
{
    return 2;
}

QVariant FrequenciesTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        QPair<QString, float> pair = m_data.at(index.column());

        if (index.row() == 0) {
            return pair.first;
        }
        else if (index.row() == 1) {
            return pair.second;
        }
    }
    else if (role == Qt::BackgroundColorRole) {
        if (index.row() == 0) {
            return QColor(Qt::darkGray);
        }
        return QColor(Qt::white);
    }
    else if (role == Qt::ForegroundRole) {
        if (index.row() == 0) return QColor(Qt::black);
    }
    else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

bool FrequenciesTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int col = index.column();
        int row = index.row();

        QPair<QString, float> p = m_data.value(col);

        if (row == 0) {
            if (m_sendSignal) {
                QString from = p.first;
                QString to = value.toString();
                emit (oldDataChanged(from, to));
            }
            p.first = value.toString();
        }
        else if (row == 1) {
            p.second = value.toFloat();
        }
        else {
            return false;
        }

        m_data.replace(col, p);
        emit (dataChanged(index, index));

        return true;
    }

    return false;
}

Qt::ItemFlags FrequenciesTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() == 1) {
        return QAbstractTableModel::flags(index) & Qt::ItemIsSelectable;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool FrequenciesTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    for (int i = column; i < column + count; i++) {
        m_data.push_back(QPair<QString, float>(QString::number(i), 0));
    }
    endInsertColumns();
    return true;
}

bool FrequenciesTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    for (int i = column; i < column + count; i++) {
        m_data.remove(i);
    }
    endRemoveColumns();
    return true;
}

void FrequenciesTableModel::addRecord(QPair<QString, float> data)
{
    m_data.append(data);
}

void FrequenciesTableModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void FrequenciesTableModel::setSendSignal(bool sendSignal)
{
    m_sendSignal = sendSignal;
}
