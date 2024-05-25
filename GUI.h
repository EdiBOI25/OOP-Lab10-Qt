#pragma once
#include <algorithm>

#include <QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableWidget>

#include "CustomQWidgets.h"
#include "service.h"
#include "CustomQWidgets.h"
#include "CartGUI.h"

class GUI: public QWidget{
private:
	Service& serv;

	QListWidget* main_list = new QListWidget{};
	QTableWidget* main_table = new QTableWidget{};

	QPushButton* add_btn = new QPushButton{"Add new subject"};
	QPushButton* sort1_btn = new QPushButton{"Sort by name"};
	QPushButton* sort2_btn = new QPushButton{"Sort by hours"};
	QPushButton* sort3_btn = new QPushButton{"Sort by teacher/type"};
	QPushButton* filter1_btn = new QPushButton{"Filter by hours less than"};
	QPushButton* filter2_btn = new QPushButton{"Filter by teacher"};
	QPushButton* filter3_btn = new QPushButton{"Show all subjects"};

	QLabel* label_status = new QLabel{ "Status messages will be displayed here" };


	QLabel* label_name = new QLabel("");
	QLabel* label_hours = new QLabel("");
	QLabel* label_type = new QLabel("");
	QLabel* label_teacher = new QLabel("");

	QPushButton* edit_btn = new QPushButton{"Edit"};
	QPushButton* remove_btn = new QPushButton{"Remove"};
	QPushButton* undo_btn = new QPushButton{"Undo"};

	QPushButton* cart_crud_btn = new QPushButton("View Cart");
	QPushButton* cart_read_btn = new QPushButton("Cart Drawing");
	QPushButton* cart_generate_btn = new QPushButton("Generate Contract");
	QPushButton* cart_clear_btn = new QPushButton("Clear Contract");

	void initGUI() {
		auto layout_main = new QHBoxLayout{};
		this->setLayout(layout_main);

		// column 1
		auto col1 = new QVBoxLayout{};
		layout_main->addLayout(col1);

		col1->addWidget(this->main_table);
		this->main_table->setColumnCount(4);
		this->main_table->setRowCount(this->serv.getAll().size());

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
		col1->addWidget(label_status);

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

		auto cart_layout = new QHBoxLayout{};
		col2->addRow(cart_layout);
		cart_layout->addWidget(this->cart_crud_btn);
		cart_layout->addWidget(this->cart_read_btn);
		cart_layout->addWidget(this->cart_generate_btn);
		cart_layout->addWidget(this->cart_clear_btn);
	}

	void updateList(const vector<Subject>& subjects) {
		this->main_list->clear();
		for (const auto& s : subjects) {
			main_list->addItem(QString::fromStdString(s.getName()));
		}
	}

	void updateTable(const vector<Subject>& subjects) {
		this->main_table->clear();
		this->main_table->setRowCount(subjects.size());
		int i = 0; // current row

		for (const auto& s : subjects) {
			QTableWidgetItem* name = new QTableWidgetItem{ QString::fromStdString(s.getName()) };
			QTableWidgetItem* hours = new QTableWidgetItem{ QString::number(s.getHours()) };
			QTableWidgetItem* type = new QTableWidgetItem{ QString::fromStdString(s.getType()) };
			QTableWidgetItem* teacher = new QTableWidgetItem{ QString::fromStdString(s.getTeacher()) };

			this->main_table->setItem(i, 0, name);
			this->main_table->setItem(i, 1, hours);
			this->main_table->setItem(i, 2, type);
			this->main_table->setItem(i, 3, teacher);
			++i;
		}
	}

	void connectSignalsSlots() {
		connect(this->main_list, &QListWidget::itemClicked, this, &GUI::showSubjectStats);

		connect(this->remove_btn, &QPushButton::clicked, this, &GUI::removeSubject);
		connect(this->undo_btn, &QPushButton::clicked, this, &GUI::undo);
		connect(this->add_btn, &QPushButton::clicked, this, &GUI::addSubject);
		connect(this->edit_btn, &QPushButton::clicked, this, &GUI::updateSubject);

		connect(this->sort1_btn, &QPushButton::clicked, this, &GUI::sortByName);
		connect(this->sort2_btn, &QPushButton::clicked, this, &GUI::sortByHours);
		connect(this->sort3_btn, &QPushButton::clicked, this, &GUI::sortByTeacher);

		connect(this->filter1_btn, &QPushButton::clicked, this, &GUI::filterByHour);
		connect(this->filter2_btn, &QPushButton::clicked, this, &GUI::filterByTeacher);
		connect(this->filter3_btn, &QPushButton::clicked, [&] {
			this->updateList(this->serv.getAll());
			updateStatus("Showing all subjects");
		});

		// cart stuff
		connect(cart_crud_btn, &QPushButton::clicked, [&] {
			auto w = new CartCRUDGUI{this->serv};
			w->show();
		});
		connect(cart_read_btn, &QPushButton::clicked, [&] {
			auto w = new CartReadOnlyGUI{ this->serv };
			w->show();
		});
		connect(cart_generate_btn, &QPushButton::clicked, [&] {
			int ss = this->serv.size();
			int random_nr = rand() % ss - 1;
			if (random_nr <= 0)
				random_nr = 1;
			this->serv.generateRandomContract(random_nr);
		});
		connect(cart_clear_btn, &QPushButton::clicked, [&] {
			this->serv.clearContract();
		});
	}

	void showSubjectStats(const QListWidgetItem* item) {
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

	void updateStatus(const string& message) {
		label_status->setText(QString::fromStdString(message));
	}

	void clearRightLabels() {
		label_name->setText("");
		label_type->setText("");
		label_hours->setText("");
		label_teacher->setText("");
	}

	void removeSubject() {
		try {
			const auto name = this->main_list->currentItem()->text().toStdString();
			const auto index = this->serv.findSubjectByName(name);
			this->serv.removeSubject(index);
			updateList(this->serv.getAll());
			updateTable(this->serv.getAll());
			clearRightLabels();
			updateStatus("Subject removed");
		}
		catch (const std::exception& e) {
			this->updateStatus(e.what());
		}

	}

	void undo() {
		try {
			this->serv.undo();
			updateList(this->serv.getAll());
			updateTable(this->serv.getAll());
			clearRightLabels();
			this->updateStatus("Undo successful");
		}
		catch (const std::exception& e) {
			this->updateStatus(e.what());
		}
	}

	void addSubject() {
		try {
			string name{};
			int hours;
			string type{};
			string teacher{};

			SubjectInputDialogWindow dialog{"", 1, "", ""};
			dialog.setWindowTitle("Add subject");
			if (dialog.exec() == QDialog::Accepted) {
				name = dialog.getName();
				hours = dialog.getHours();
				type = dialog.getType();
				teacher = dialog.getTeacher();
			}
			else return;

			this->serv.addSubject(name, hours, type, teacher);
			updateList(this->serv.getAll());
			updateTable(this->serv.getAll());
			clearRightLabels();
			updateStatus("Added subject");
		}
		catch (const std::exception& e) {
			updateStatus(e.what());
		}
	}

	void updateSubject() {
		try {
			string name{};
			int hours;
			string type{};
			string teacher{};

			string current_name = this->main_list->currentItem()->text().toStdString();
			const int index = this->serv.findSubjectByName(current_name);
			const Subject s = this->serv.getAll()[index];

			SubjectInputDialogWindow dialog{ QString::fromStdString(s.getName()),
				s.getHours(),
				QString::fromStdString(s.getType()),
				QString::fromStdString(s.getTeacher())};
			dialog.setWindowTitle("Edit subject");
			if (dialog.exec() == QDialog::Accepted) {
				name = dialog.getName();
				hours = dialog.getHours();
				type = dialog.getType();
				teacher = dialog.getTeacher();
			}
			else return;

			
			this->serv.updateSubject(index, name, hours, type, teacher);
			updateList(this->serv.getAll());
			updateTable(this->serv.getAll());
			clearRightLabels();
			updateStatus("Updated subject");
		}
		catch (const std::exception& e) {
			updateStatus(e.what());
		}
	}

	void sortByName() {
		try {
			auto result = this->serv.getAll();
			std::sort(result.begin(), result.end(), [&](const Subject& s1, const Subject& s2) {
				return s1.getName() < s2.getName();
				});
			updateList(result);
			updateStatus("List sorted");
		}
		catch (const std::exception& e) {
			updateStatus(e.what());
		}
	}

	void sortByHours() {
		try {
			auto result = this->serv.getAll();
			std::sort(result.begin(), result.end(), [&](const Subject& s1, const Subject& s2) {
				return s1.getHours() < s2.getHours();
				});
			updateList(result);
			updateStatus("List sorted");
		}
		catch (const std::exception& e) {
			updateStatus(e.what());
		}
	}

	void sortByTeacher() {
		try {
			auto result = this->serv.getAll();
			std::sort(result.begin(), result.end(), [&](const Subject& s1, const Subject& s2) {
				return (s1.getTeacher() < s2.getTeacher()) || (s1.getTeacher() == s2.getTeacher() && s1.getType() < s2.getType());
				});
			updateList(result);
			updateStatus("List sorted");
		}
		catch (const std::exception& e) {
			updateStatus(e.what());
		}
	}

	void filterByHour() {
		try {
			int hours;
			FilterDialogWindow dialog{ "Enter number of hours per week", true };
			dialog.setWindowTitle("Filter by hours");
			if (dialog.exec() == QDialog::Accepted) {
				hours = dialog.getValue().toInt();
			}
			else return;

			auto all = this->serv.getAll();
			vector<Subject> result{};
			std::copy_if(all.begin(), all.end(), std::back_inserter(result), [&](const Subject& s) {
				return s.getHours() <= hours;
				});
			updateList(result);
			updateStatus("List filtered");
		}
		catch (const std::exception& e) {
			updateStatus(e.what());
		}
	}

	void filterByTeacher() {
		try {
			string teacher = "dude";
			FilterDialogWindow dialog{ "Enter teacher", false };
			dialog.setWindowTitle("Filter by teacher name");
			if (dialog.exec() == QDialog::Accepted) {
				teacher = dialog.getValue().toStdString();
			}
			else return;

			auto all = this->serv.getAll();
			vector<Subject> result{};
			std::copy_if(all.begin(), all.end(), std::back_inserter(result), [&](const Subject& s) {
				return s.getTeacher().find(teacher) != string::npos;
				});
			updateList(result);
			updateStatus("List filtered");
		}
		catch (const std::exception& e) {
			updateStatus(e.what());
		}
	}
public:
	GUI(Service& s): serv{s} {
		srand(time(NULL));
		initGUI();
		updateList(this->serv.getAll());
		updateTable(this->serv.getAll());
		connectSignalsSlots();
	}	
};


