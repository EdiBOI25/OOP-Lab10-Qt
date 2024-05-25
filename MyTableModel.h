#pragma once

#include <QAbstractTableModel>

class MyTableModel : public QAbstractTableModel{
	Q_OBJECT
private:
	QString m_gridData[100][4];
public:
	MyTableModel(QObject* parent);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;

	void updateTable(const QVector<QVector<QVariant>>& new_data) {
		for(int i = 0; i < new_data.size(); ++i) {
			for(int j = 0; j < new_data[i].size(); ++j) {
				const auto& elem = new_data[i][j];
				QModelIndex index = createIndex(i, j);
				this->setData(index, elem, Qt::EditRole);
			}
		}
	}
};
