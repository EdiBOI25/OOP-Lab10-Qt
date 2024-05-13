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

	QLabel* label_name = new QLabel("name test ");
	QLabel* label_hours = new QLabel("269 ");
	QLabel* label_type = new QLabel("cacamaca ");
	QLabel* label_teacher = new QLabel("ceva baiat ");

	QPushButton* edit_btn = new QPushButton{"Edit"};
	QPushButton* remove_btn = new QPushButton{"Remove"};
	QPushButton* undo_btn = new QPushButton{"Undo"};
public:
	GUI(Service& s): serv{s} {
		initGUI();
		refreshList(this->serv.getAll());
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

	void refreshList(const vector<Subject>& subjects) {
		for (const auto& s : subjects) {
			main_list->addItem(QString::fromStdString(s.getName()));
			//layout_main->addWidget(label);
			// list->addItem(label) cred sau cv de genu
		}
	}
};
