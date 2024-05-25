#include "MyTableModel.h"

#include <string>

using std::string;

MyTableModel::MyTableModel(QObject* parent) : QAbstractTableModel(parent) {}

int MyTableModel::rowCount(const QModelIndex& parent) const {
	return 100;
}

int MyTableModel::columnCount(const QModelIndex& parent) const {
	return 4;
}

QVariant MyTableModel::data(const QModelIndex& index, int role) const {
	int row = index.row();
	int col = index.column();
	if (role == Qt::DisplayRole) {
		return this->m_gridData[row][col];
	}

	return QVariant();
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch(section) {
			case 0:
				return QString("Name");
			case 1:
				return QString("Hours per week");
			case 2:
				return QString("Type");
			case 3:
				return QString("Teacher");
			default:
				return QString::number(section);
			}
		} else {
			return QString::number(section + 1);
		}
	}

	return QVariant();
}

bool MyTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (role == Qt::EditRole) {
		int row = index.row();
		int col = index.column();

		m_gridData[index.row()][index.column()] = value.toString();
		QModelIndex qmi = createIndex(row, col);
		emit dataChanged(qmi, qmi);
	}

	return QAbstractTableModel::setData(index, value, role);
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex& index) const {
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}
