#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QDialog>

class SliderWithLabel : public QWidget {
private:
	QSlider* slider = new QSlider{Qt::Horizontal};
	QLabel* value_label = new QLabel{ "" };

public:
	SliderWithLabel(int value) {
		auto layout_main = new QHBoxLayout{};
		this->setLayout(layout_main);

		slider->setMinimum(1);
		slider->setMaximum(10);
		slider->setValue(value);
		value_label->setText(QString::number(value));

		layout_main->addWidget(slider);
		layout_main->addWidget(value_label);

		connect(slider, &QSlider::valueChanged, this, [&] {
			value_label->setText(QString::number(slider->value()));
			});
	}

	int getValue() const {
		return slider->value();
	}

	void setValue(int value) {
		slider->setValue(value);
	}
};

class QComboBoxForTypes : public QComboBox {
public:
	QComboBoxForTypes() {
		this->insertItem(0, "Compulsory");
		this->insertItem(1, "Optional");
		this->insertItem(2, "Other");
	}
};

class FilterDialogWindow : public QDialog {
private:
	QLineEdit* input_line = new QLineEdit{""};
	QLabel* is_ok = new QLabel{ "" };
	QPushButton* ok_btn = new QPushButton{ "OK" };
	QPushButton* cancel_btn = new QPushButton{ "Cancel" };
	std::string message{};
	bool must_be_int{};

	bool isStringNumber(const std::string& s) {
		for (const auto letter : s) {
			if (!isdigit(letter)) {
				return false;
			}
		}
		return true;
	}

	void validate_data() {
		if (this->getValue().toStdString().empty()) {
			this->is_ok->setText("Value must not be empty");
			this->is_ok->setStyleSheet("QLabel { color : red; }");
			this->ok_btn->setEnabled(false);
			return;
		}
		if (this->must_be_int) {
			try {
				if (isStringNumber(this->getValue().toStdString()) == false) {
					throw std::exception();
				}
				this->is_ok->clear();
				this->ok_btn->setEnabled(true);
			}
			catch (const std::exception&) {
				this->is_ok->setText("Value must be a number");
				this->is_ok->setStyleSheet("QLabel { color : red; }");
				this->ok_btn->setEnabled(false);
			}
		}
		else {
			this->is_ok->clear();
			this->ok_btn->setEnabled(true);
		}
	}
public:
	FilterDialogWindow(const std::string& m, bool mbi) : message{ m }, must_be_int{ mbi } {
		auto layout = new QVBoxLayout{};
		this->setLayout(layout);

		auto btns = new QHBoxLayout{};
		this->ok_btn->setEnabled(false);
		btns->addWidget(ok_btn);
		btns->addWidget(cancel_btn);

		layout->addWidget(new QLabel(QString::fromStdString(message)));
		layout->addWidget(input_line);
		layout->addWidget(is_ok);
		layout->addLayout(btns);

		connect(ok_btn, &QPushButton::clicked, this, &FilterDialogWindow::accept);
		connect(cancel_btn, &QPushButton::clicked, this, &FilterDialogWindow::reject);
		connect(input_line, &QLineEdit::textChanged, this, &FilterDialogWindow::validate_data);
	}

	QString getValue() {
		return input_line->text();
	}	
};

class SubjectInputDialogWindow : public QDialog {
private:
	QLineEdit* input_name = new QLineEdit{};
	QLabel* label_is_name_ok = new QLabel{ "" };
	SliderWithLabel* slider_hours = new SliderWithLabel{3};
	QComboBoxForTypes* box_types = new QComboBoxForTypes{};
	QLineEdit* input_teacher = new QLineEdit{};
	QLabel* label_is_teacher_ok = new QLabel{ "" };

	QPushButton* ok_btn = new QPushButton{ "OK" };
	QPushButton* cancel_btn = new QPushButton{ "Cancel" };

	void validateName() {
		if (this->getName().empty()) {
			this->label_is_name_ok->setText("Value must not be empty");
			this->label_is_name_ok->setStyleSheet("QLabel { color : red; }");
			this->ok_btn->setEnabled(false);
		}
		else {
			this->label_is_name_ok->clear();
			if (!this->getTeacher().empty())
				this->ok_btn->setEnabled(true);
		}
	}

	void validateTeacher() {
		if (this->getTeacher().empty()) {
			this->label_is_teacher_ok->setText("Value must not be empty");
			this->label_is_teacher_ok->setStyleSheet("QLabel { color : red; }");
			this->ok_btn->setEnabled(false);
		}
		else {
			this->label_is_teacher_ok->clear();
			if (!this->getName().empty())
				this->ok_btn->setEnabled(true);
		}
	}
public:
	SubjectInputDialogWindow(const QString& name, int hours, const QString& type, const QString& teacher) {
		auto layout_main = new QVBoxLayout{};
		this->setLayout(layout_main);

		input_name->setText(name);
		slider_hours->setValue(hours);
		if (type == "compulsory")
			box_types->setCurrentIndex(0);
		else if (type == "optional")
			box_types->setCurrentIndex(1);
		else box_types->setCurrentIndex(2);
		input_teacher->setText(teacher);

		auto btns = new QHBoxLayout{};
		if (name.isEmpty() || teacher.isEmpty())
			this->ok_btn->setEnabled(false);
		else
			this->ok_btn->setEnabled(true);
		btns->addWidget(ok_btn);
		btns->addWidget(cancel_btn);

		layout_main->addWidget(new QLabel("Name"));
		layout_main->addWidget(input_name);
		layout_main->addWidget(label_is_name_ok);
		layout_main->addWidget(new QLabel("Hours per week"));
		layout_main->addWidget(slider_hours);
		layout_main->addWidget(new QLabel("Type"));
		layout_main->addWidget(box_types);
		layout_main->addWidget(new QLabel("Teacher"));
		layout_main->addWidget(input_teacher);
		layout_main->addWidget(label_is_teacher_ok);
		layout_main->addLayout(btns);

		connect(ok_btn, &QPushButton::clicked, this, &SubjectInputDialogWindow::accept);
		connect(cancel_btn, &QPushButton::clicked, this, &SubjectInputDialogWindow::reject);
		connect(input_name, &QLineEdit::textChanged, this, &SubjectInputDialogWindow::validateName);
		connect(input_teacher, &QLineEdit::textChanged, this, &SubjectInputDialogWindow::validateTeacher);
	}

	std::string getName() const{
		return this->input_name->text().toStdString();
	}

	std::string getTeacher() const{
		return this->input_teacher->text().toStdString();
	}

	int getHours() const{
		return this->slider_hours->getValue();
	}

	std::string getType() const {
		if (this->box_types->currentIndex() == 0) {
			return "compulsory";
		}
		if (this->box_types->currentIndex() == 1) {
			return "optional";
		}
		return "other";
	}
};