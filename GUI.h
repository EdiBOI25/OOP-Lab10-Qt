#pragma once
#include <QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFormLayout>

#include "service.h"

class GUI: public QWidget {
private:
	Service& serv;

	QListWidget* main_list = new QListWidget{};

	QPushButton* add_btn = new QPushButton{"Add new subject"};
	QPushButton* sort1_btn = new QPushButton{"Sort1"};
	QPushButton* sort2_btn = new QPushButton{"Sort2"};
	QPushButton* sort3_btn = new QPushButton{"Sort3"};
	QPushButton* filter1_btn = new QPushButton{"filter1"};
	QPushButton* filter2_btn = new QPushButton{"filter2"};
	QPushButton* filter3_btn = new QPushButton{"filter3"};


	QLabel* label_name = new QLabel("");
	QLabel* label_hours = new QLabel("");
	QLabel* label_type = new QLabel("");
	QLabel* label_teacher = new QLabel("");

	QPushButton* edit_btn = new QPushButton{"Edit"};
	QPushButton* remove_btn = new QPushButton{"Remove"};
	QPushButton* undo_btn = new QPushButton{"Undo"};
public:
	GUI(Service& s): serv{s} {
		initGUI();
		updateList(this->serv.getAll());
		connectSignalsSlots();
	}

	void initGUI() {
		auto layout_main = new QHBoxLayout{};
		this->setLayout(layout_main);

		// column 1
		auto col1 = new QVBoxLayout{};
		layout_main->addLayout(col1);

		col1->addWidget(this->main_list);
		col1->addWidget(this->add_btn);
		auto sort_layout = new QHBoxLayout{};
		col1->addLayout(sort_layout);
		sort_layout->addWidget(this->sort1_btn);
		sort_layout->addWidget(this->sort2_btn);
		sort_layout->addWidget(this->sort3_btn);
		auto filter_layout = new QHBoxLayout{};
		col1->addLayout(filter_layout);
		filter_layout->addWidget(this->filter1_btn);
		filter_layout->addWidget(this->filter2_btn);
		filter_layout->addWidget(this->filter3_btn);

		// column 2
		auto col2 = new QFormLayout{};
		layout_main->addLayout(col2);

		col2->addRow("Name: ", label_name);
		col2->addRow("Hours per week: ", label_hours);
		col2->addRow("Type: ", label_type);
		col2->addRow("Teacher: ", label_teacher);

		auto actions_layout = new QHBoxLayout{};
		//col2->addLayout(actions_layout);
		col2->addRow(actions_layout);
		actions_layout->addWidget(this->edit_btn);
		actions_layout->addWidget(this->remove_btn);
		actions_layout->addWidget(this->undo_btn);
	}

	void updateList(const vector<Subject>& subjects) {
		this->main_list->clear();
		for (const auto& s : subjects) {
			main_list->addItem(QString::fromStdString(s.getName()));
			//layout_main->addWidget(label);
			// list->addItem(label) cred sau cv de genu
		}
	}

	void connectSignalsSlots() {
		connect(this->main_list, &QListWidget::itemClicked, this, &GUI::showSubjectStats);
		connect(this->remove_btn, &QPushButton::clicked, this, &GUI::removeSubject);
	}

	void showSubjectStats(const QListWidgetItem *item) {
		if (item) {
			const auto name = item->text().toStdString();
			const auto index = this->serv.findSubjectByName(name);
			if (index == -1) {
				throw std::exception("subject not found");
			}
			const auto subject = this->serv.getAll()[index];
			label_name->setText(QString::fromStdString(subject.getName()));
			label_hours->setText(QString::fromStdString(std::to_string(subject.getHours())));
			label_type->setText(QString::fromStdString(subject.getType()));
			label_teacher->setText(QString::fromStdString(subject.getTeacher()));
		}
	}

	void removeSubject() {
		const auto name = this->main_list->currentItem()->text().toStdString();
		const auto index = this->serv.findSubjectByName(name);
		this->serv.removeSubject(index);
		updateList(this->serv.getAll());
		label_name->setText("");
		label_type->setText("");
		label_hours->setText("");
		label_teacher->setText("");
	}
};
