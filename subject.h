#pragma once
#include <string>

using std::string;

class Subject {
private:
	string name{};
	int hours_per_week{0};
	string type{}; // compulsory or optional
	string teacher{};

public:
	// Constructor for empty subject
	Subject() = default;

	/**
	 * \brief Subject class constructor: initializes subject
	 * \param name name of subject
	 * \param hours_per_week hours per week of subject
	 * \param type type of subject (compulsory, optional or other)
	 * \param teacher name of teacher who teaches the subject
	 */
	Subject(const string& name, const int& hours_per_week, const string& type, const string& teacher);

	/**
	 * \brief Copy constructor
	 * \param s subject to copy
	 */
	Subject(const Subject& s);

	/**
	 * \brief Subject class destructor
	 */
	~Subject() = default;

	/**
	 * \brief Returns the name of the subject
	 * \return name of subject
	 */
	string getName() const;

	/**
	 * \brief Returns the number of hours per week of the subject
	 * \return hours per week of subject
	 */
	int getHours() const noexcept;

	/**
	 * \brief Returns the type of the subject
	 * \return type of subject
	 */
	string getType() const;

	/**
	 * \brief Returns the name of the teacher of the subject
	 * \return name of teacher of subject
	 */
	string getTeacher() const;

	/**
	 * \brief Sets new name of subject
	 * \param new_name new name to replace the old one
	 */
	void setName(const string& new_name);
	/**
	 * \brief Sets a new number of hours per week
	 * \param new_hours new number of hours per week
	 */
	void setHours(const int& new_hours) noexcept;
	/**
	 * \brief Sets new type of subject
	 * \param new_type new type to replace the old one
	 */
	void setType(const string& new_type);
	/**
	 * \brief Sets new teacher of subject
	 * \param new_teacher new teacher name to replace the old one
	 */
	void setTeacher(const string& new_teacher);

	friend std::ostream& operator << (std::ostream& out, const Subject& subject);

	bool operator==(const Subject& other) const noexcept;

	bool operator!=(const Subject& other) const noexcept;
};
