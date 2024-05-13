#pragma once
#include <functional>
#include <map>

#include "repository.h"
#include "Cart.h"
#include "Undo.h"

using std::unique_ptr;

struct DTO {
	string type{};
	int count{ 0 };
};
DTO operator++(DTO& dto);

class Service {
private:
	// Repository& repo;
	AbstractRepo& repo;
	Cart contract;
	vector<unique_ptr<UndoAction>> undo_list;

public:

	/**
	 * \brief Service constructor
	 * \param repo repository to hold elements
	 */
	Service(AbstractRepo& repo);

	virtual ~Service() = default;

	/**
	 * \brief Returns all elements in list
	 * \return list of elements
	 */
	const std::vector<Subject> getAll() const noexcept;

	/**
	 * \brief Adds a subject to the list
	 * \param name name of subject
	 * \param hours number of hours per week of subject
	 * \param type type of subject
	 * \param teacher teacher's name of subject
	 */
	void addSubject(const string& name, const int& hours, const string& type, const string& teacher);

	/**
	 * \brief Returns the number of elements in list
	 * \return number of elements in list
	 */
	size_t size() const noexcept;

	/**
	 * \brief Returns the position of a subject in the list
	 * \param name name of subject
	 * \param hours number of hours per week of subject
	 * \param type type of subject
	 * \param teacher teacher of subject
	 * \return position at which subject is found or -1 otherwise
	 */
	int findSubject(const string& name, const int& hours, const string& type, const string& teacher) const;

	// finds subject by given name
	// returns -1 if not found
	int findSubjectByName(const string& name) const{
		const auto subjects = this->getAll();
		for(int i = 0; i < subjects.size(); ++i) {
			if (subjects[i].getName() == name) {
				return i;
			}
		}
		return -1;
	}

	/**
	 * \brief Removes the subject at a given index
	 * \param index index of subject to remove
	 */
	void removeSubject(const int& index);

	/**
	 * \brief Updates subject at a given index with new parameters
	 * \param index index of element to replace with new one
	 * \param name new name
	 * \param hours new number of hours per week
	 * \param type new type
	 * \param teacher new teacher name
	 */
	void updateSubject(const int& index, const string& name, const int& hours, const string& type, const string& teacher);

	friend std::ostream& operator<<(std::ostream& out, const Service& serv);

	/**
	 * \brief Filters list with given method
	 * \param condition method to filter by
	 * \return filtered list
	 */
	std::vector<Subject> filter(const std::function<bool(const Subject& subject)>& condition) const;

	/**
	 * \brief Sorts and returns list
	 * \param condition method to sort by
	 * \param reverse whether sorting should be in reverse order
	 * \return sorted list
	 */
	std::vector<Subject> sort(const std::function<bool(const Subject& s1, const Subject& s2)>& condition, bool reverse) const;

	/**
	 * \brief Add subjects with given name to contract
	 * \param name name of subject
	 */
	void addToContract(const string& name);

	/**
	 * \brief Empties contract
	 */
	void clearContract() noexcept;

	/**
	 * \brief Adds random subjects to contract
	 * \param number number of subjects
	 */
	void generateRandomContract(const int& number);

	void exportContract(const string& file_name) const;

	size_t contractSize() const;

	/**
	 * \brief Get count for each type of subject
	 * \return Subject type and their respective DTOs containing type and count
	 */
	std::map<string, DTO> reportByType() const;

	void undo();

	void printContract() const;
};
