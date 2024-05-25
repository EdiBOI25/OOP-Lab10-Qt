#pragma once

#include <QWidget>
#include <QListWidget>
#include <QLayout>
#include <QPushButton>
#include <QPainter>

#include "Observer.h"
#include "service.h"

class CartCRUDGUI : public QWidget, public Observer {
private:
	Service& serv;
	QListWidget* list = new QListWidget{};
	QPushButton* generate_btn = new QPushButton{ "Generate Contract" };
	QPushButton* clear_btn = new QPushButton{ "Clear Contract" };

	void reloadList(const vector<Subject>& l) {
		this->list->clear();
		for(const auto& el: l) {
			list->addItem(QString::fromStdString(el.getName()));
		}
	}

	void connectSignals() {
		connect(generate_btn, &QPushButton::clicked, [&] {
			int ss = this->serv.size();
			int random_nr = rand() % ss - 1;
			if (random_nr <= 0)
				random_nr = 1;
			this->serv.generateRandomContract(random_nr);
		});
		connect(clear_btn, &QPushButton::clicked, [&] {
			this->serv.clearContract();
			});
	}
public:
	CartCRUDGUI(Service& s) : serv(s) {
		this->setWindowTitle("Contract CRUD GUI");
		this->serv.addObserver(this);

		auto ly = new QHBoxLayout{};
		this->setLayout(ly);

		ly->addWidget(list);
		ly->addWidget(generate_btn);
		ly->addWidget(clear_btn);
		reloadList(serv.getContract());

		connectSignals();
	}

	void obsUpdate() override {
		// pt fiecare elem din service, populam lista
		reloadList(serv.getContract());
	}
};

class CartReadOnlyGUI : public QWidget, public Observer {
private:
	Service& serv;
	int size;

public:
	CartReadOnlyGUI(Service& s) : serv(s) {
		this->setWindowTitle("Contract ReadOnly GUI");
		this->serv.addObserver(this);
		size = serv.contractSize();
	}

	void obsUpdate() override {
		size = serv.contractSize();
		repaint();
	}

protected:
	void paintEvent(QPaintEvent* event) override {
		QPainter p{ this };

		/*p.drawLine(0, 0, width(), height());

		p.setPen(Qt::green);
		p.setFont(QFont("Arial", 30));
		p.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, "Qt QPainter");*/

		for(int i = 0; i < size; ++i)
			p.fillRect(rand() % width(), rand() % height(), 100, 100, Qt::BrushStyle::Dense1Pattern);
	}
};
