#include "subject.h"

#include <fstream>
#include <iostream>

Subject::Subject(const string& name, const int& hours_per_week, const string& type, const string& teacher) {
	this->name = name;
	this->hours_per_week = hours_per_week;
	this->type = type;
	this->teacher = teacher;
}

Subject::Subject(const Subject& s) {
	this->name = s.name;
	this->hours_per_week = s.hours_per_week;
	this->type = s.type;
	this->teacher = s.teacher;
}

//Subject::~Subject() {
//	// destructor
//}

string Subject::getName() const {
	return this->name;
}

int Subject::getHours() const noexcept{
	return this->hours_per_week;
}

string Subject::getType() const {
	return this->type;
}

string Subject::getTeacher() const {
	return this->teacher;
}

void Subject::setName(const string& new_name) {
	this->name = new_name;
}

void Subject::setHours(const int& new_hours) noexcept{
	this->hours_per_week = new_hours;
}

void Subject::setType(const string& new_type) {
	this->type = new_type;
}

void Subject::setTeacher(const string& new_teacher) {
	this->teacher = new_teacher;
}

std::ostream& operator<<(std::ostream& out, const Subject& subject) {
	out << "Name: " << subject.name << "\tHours per week: " << subject.hours_per_week << "\tType: " << subject.type << "\tTeacher: " << subject.teacher;
	return out;
}

bool Subject::operator==(const Subject& other) const noexcept{
	return this->name == other.name && this->hours_per_week == other.hours_per_week && this->type == other.type && this->teacher== other.teacher;
	/*if (this->name != other.name) {
		return false;
	}
	if (this->hours_per_week != other.hours_per_week) {
		return false;
	}
	if (this->type != other.type) {
		return false;
	}
	if (this->teacher != other.teacher) {
		return false;
	}

	return true;*/
}

bool Subject::operator!=(const Subject& other) const noexcept{
	return this->name != other.name || this->hours_per_week != other.hours_per_week || this->type != other.type || this->teacher != other.teacher;
	/*if (this->name != other.name) {
		return true;
	}
	if (this->hours_per_week != other.hours_per_week) {
		return true;
	}
	if (this->type != other.type) {
		return true;
	}
	if (this->teacher != other.teacher) {
		return true;
	}

	return false;*/
}