#pragma once
#include <algorithm>
#include <iostream>
#include <random>
#include <cstdlib>
#include <vector>
//#include "DynamicArray.h"
#include "subject.h"
#include <fstream>
#include <map>
#include <sstream>

typedef Subject Element;
using std::vector;
using std::string;

class AbstractRepo {
public:
	virtual void add(const Element& e) = 0;

	virtual const std::vector<Element> getAll() const noexcept = 0;

	virtual int find(const Element& element) const = 0;

	virtual void remove(const Element& element) = 0;

	virtual void update(const Element& old_elem, const Element& element) = 0;

	virtual size_t size() const = 0;

	virtual const Element& operator[](const int& index) const = 0;

	//virtual std::ostream& operator<<(std::ostream& out) const = 0;

	virtual ~AbstractRepo() = default;
};

class Repository : public AbstractRepo {
protected:
	std::vector<Element> list{};
public:
	/**
	 * \brief Repository constructor
	 */
	Repository() = default;

	/**
	 * \brief Repository deconstructor
	 */
	~Repository() override = default;

	/**
	 * \brief Returns the list of elements in repository
	 * \return list of elements in repository
	 */
	const std::vector<Element> getAll() const noexcept override{
		return this->list;
	}
	/*const std::vector<Element>* getAll() const noexcept{
		return &this->list;
	}*/

	/**
	 * \brief Adds an element to the list
	 * \param element the element to be added
	 */
	void add(const Element& element) override{
		this->list.push_back(element);
	}


	/**
	 * \brief Returns the first foun position of the element in the list
	 * \param element element to search for
	 * \return position of first found element or -1 if not found
	 */
	int find(const Element& element) const override{
		/*for (int i = 0; i < this->list.size(); i++) {
			if (this->list.at(i) == element) {
				return i;
			}
		}
		return  -1;*/

		const auto it = std::find(this->list.begin(), this->list.end(), element);
		if (it != this->list.end()) {
			return static_cast<int>(std::distance(this->list.begin(), it));
		}
		return -1;
	}

	/**
	 * \brief Removes an element from the list
	 * \param element element to remove
	 */
	void remove(const Element& element) override{
		const int index = this->find(element);
		if (index == -1) {
			throw std::exception("Couldn't find element to remove");
		}
		this->list.erase(this->list.begin() + index);
	}

	/**
	 * \brief Removes an element found at given index
	 * \param index index of element to remove
	 */
	virtual void remove(const size_t& index) {
		if (index < 0 || index >= this->list.size()) {
			throw std::out_of_range("Index out of range");
		}
		this->list.erase(this->list.begin() + index);
	}

	/**
	 * \brief Replaces element found at given index with a new one
	 * \param old_elem old element to update
	 * \param element element to replace the old one with
	 */
	void update(const Element& old_elem, const Element& element) override{
		const int index = this->find(old_elem);

		if (index < 0 || index >= this->list.size()) {
			throw std::out_of_range("Index out of range");
		}
		this->list[index] = element;
	}

	/**
	 * \brief Returns the size of the repository list
	 * \return size of repository list
	 */
	size_t size() const noexcept override{
		return this->list.size();
	}

	const Element& operator[](const int& index) const override{
		if (index < 0 || index >= this->list.size()) {
			throw std::out_of_range("Index out of range");
		}
		return this->list[index];
	}

	//std::ostream& operator<<(std::ostream& out) const {
	//	/*for(int i = 0; i < repo.size(); ++i) {
	//		out << i << ": " << repo.list.at(i) << '\n';
	//	}*/
	//	int index = 0;
	//	for (const auto& s : this->list) {
	//		out << index << ": " << s << '\n';
	//		index++;
	//	}
	//	return out;
	//}
};


class RepoWithFile: public Repository {
private:
	string file_name;
	void loadFromFile() {
		std::ifstream file(this->file_name);
		if(!file.is_open()) {
			throw std::exception("File not found");
		}		

		string line{};
		while(std::getline(file, line)) {
			if(line.empty()) {
				continue;
			}
			std::stringstream ss(line);
			string token{};
			std::vector<string> params{};
			while(std::getline(ss, token, ';')) {
				params.push_back(token);
			}

			const Subject s(params[0], std::stoi(params[1]), params[2], params[3]);
			Repository::add(s);
		}
	}

	void storeToFile() const{
		std::ofstream fout(this->file_name);
		for(const auto& s: this->list) {
			fout << s.getName() << ";" << s.getHours() << ";" << s.getType() << ";" << s.getTeacher() << '\n';
		}
		fout.close();
	}
public:
	RepoWithFile(const string& file) : Repository() {
		this->file_name = file;
		loadFromFile();
	}

	void add(const Element& element) override {
		Repository::add(element);
		storeToFile();
	}

	void remove(const Element& element) override {
		Repository::remove(element);
		storeToFile();
	}

	void remove(const size_t& index) override {
		Repository::remove(index);
		storeToFile();
	}

	void update(const Element& old_elem, const Element& element) override {
		Repository::update(old_elem, element);
		storeToFile();
	}
};

class RepoLab : public AbstractRepo {
private:
	std::map<int, Element> list{};
	double probability = 0.0;
public:
	/**
	 * \brief Repository constructor
	 */
	RepoLab(const double& p) : probability{ p } {}

	/**
	 * \brief Repository deconstructor
	 */
	~RepoLab() override = default;

	/**
	 * \brief Returns the list of elements in repository
	 * \return list of elements in repository
	 */
	const std::vector<Element> getAll() const noexcept override {
		vector<Element> all;

		for(const auto& pair: this->list) {
			all.push_back(pair.second);
		}

		return all;
	}

	/**
	 * \brief Adds an element to the list
	 * \param element the element to be added
	 */
	void add(const Element& element) override {
		const double random_num = rand() % 10 / static_cast<double>(10);
		if (random_num < this->probability) {
			throw std::exception("Generated number is lower than given probability");
		}

		//this->list.push_back(element);
		const int s = static_cast<int>(this->size());
		this->list[s] = element;
	}


	/**
	 * \brief Returns the first foun position of the element in the list
	 * \param element element to search for
	 * \return position of first found element or -1 if not found
	 */
	int find(const Element& element) const override {
		for (const auto& e: this->list) {
			if (e.second == element) {
				return e.first;
			}
		}
		return  -1;

		/*const auto it = std::find(this->list.begin(), this->list.end(), element);
		if (it != this->list.end()) {
			return static_cast<int>(std::distance(this->list.begin(), it));
		}
		return -1;*/
	}

	/**
	 * \brief Removes an element from the list
	 * \param element element to remove
	 */
	void remove(const Element& element) override {
		const int index = this->find(element);
		if (index == -1) {
			throw std::exception("Couldn't find element to remove");
		}

		const double random_num = rand() % 10 / static_cast<double>(10);
		if (random_num < this->probability) {
			throw std::exception("Generated number is lower than given probability");
		}

		this->list.erase(index);
	}

	/**
	 * \brief Removes an element found at given index
	 * \param index index of element to remove
	 */
	virtual void remove(const size_t& index) {
		const int i = static_cast<int>(index);
		if (this->list.find(i) == this->list.end()) {
			throw std::out_of_range("Index out of range");
		}

		const double random_num = rand() % 10 / static_cast<double>(10);
		if (random_num < this->probability) {
			throw std::exception("Generated number is lower than given probability");
		}

		this->list.erase(i);
	}

	/**
	 * \brief Replaces element found at given index with a new one
	 * \param old_elem old element to update
	 * \param element element to replace the old one with
	 */
	void update(const Element& old_elem, const Element& element) override {
		const int index = this->find(old_elem);

		if (index < 0) {
			throw std::out_of_range("Element not found");
		}

		const double random_num = rand() % 10 / static_cast<double>(10);
		if (random_num < this->probability) {
			throw std::exception("Generated number is lower than given probability");
		}

		this->list[index] = element;
	}

	/**
	 * \brief Returns the size of the repository list
	 * \return size of repository list
	 */
	size_t size() const noexcept override {
		return this->list.size();
	}

	const Element& operator[](const int& index) const override {
		if (this->list.find(index) == this->list.end()) {
			throw std::out_of_range("Index out of range");
		}
		return this->list.at(index);
	}
};

inline std::ostream& operator<<(std::ostream& out, const AbstractRepo& repo) {
	int index = 0;
	const auto& all = repo.getAll();
	for (const auto& s : all) {
		out << index << ": " << s << '\n';
		index++;
	}
	return out;
}
