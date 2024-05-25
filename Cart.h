#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#include "subject.h"

using std::vector;

class Cart {
private:
	vector <Subject> list{};
public:
	Cart() = default;

	//~Cart() = default;

	/**
	 * \brief Removes everything from cart
	 */
	void clear() noexcept{
		this->list.clear();
	}

	/**
	 * \brief Adds subject to cart
	 * \param subject subject to add
	 */
	void add(const Subject& subject) {
		this->list.push_back(subject);
	}

	/**
	 * \brief Get list of subjects in cart
	 * \return list
	 */
	vector<Subject>* getAll() noexcept {
		return &this->list;
	}

	vector<Subject> getContract() {
		return this->list;
	}

	/**
	 * \brief Get number of subjects in cart
	 * \return number of subject
	 */
	size_t size() const noexcept {
		return this->list.size();
	}

	void exportToFile(string file_name) const{
		file_name += ".csv";
		std::ofstream fout(file_name);
		for (const auto& s : this->list) {
			fout << s.getName() << "," << s.getHours() << "," << s.getType() << "," << s.getTeacher() << '\n';
		}
		fout.close();
	}

	friend std::ostream& operator<<(std::ostream& out, const Cart& cart) {
		int index = 0;
		for (const auto& subject : cart.list) {
			out << index << ": " << subject << '\n';
			++index;
		}
		return out;
	}
};
