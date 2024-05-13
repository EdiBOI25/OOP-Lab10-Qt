#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>

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