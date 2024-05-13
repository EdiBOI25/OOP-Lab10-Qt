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
			this->ok_btn->setEnabled(true);
		}
	}
};