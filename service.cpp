#include "service.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

Service::Service(AbstractRepo& r) : repo{ r }, undo_list{} {
	this->contract = Cart{};
}


DTO operator++(DTO& dto) {
	dto.count++;
	return dto;
}

const std::vector<Subject> Service::getAll() const noexcept{
	return this->repo.getAll();
}

void Service::addSubject(const string& name, const int& hours, const string& type, const string& teacher) {
	Subject subject{ name, hours, type, teacher };
	if (this->repo.find(subject) != -1)
		throw std::exception("Subject already exists");
	this->repo.add(subject);

	this->undo_list.push_back(std::make_unique<UndoAdd>(subject, this->repo));
}

size_t Service::size() const noexcept{
	return this->repo.size();
}

int Service::findSubject(const string& name, const int& hours, const string& type, const string& teacher) const {
	const Subject subject{ name, hours, type, teacher };
	return this->repo.find(subject);
}

void Service::removeSubject(const int& index) {
	const Subject s = this->repo[index];
	// this->repo.remove(index);
	this->repo.remove(s);

	this->undo_list.push_back(std::make_unique<UndoRemove>(s, this->repo));
}

void Service::updateSubject(const int& index, const string& name, const int& hours, const string& type, const string& teacher) {
	const Subject subject{ name, hours, type, teacher };
	if (this->repo.find(subject) != -1)
		throw std::exception("Subject already exists");
	const Subject old_subject = this->repo[index];
	this->repo.update(old_subject, subject);
	this->undo_list.push_back(std::make_unique<UndoUpdate>(old_subject, subject, this->repo));
}

std::vector<Subject> Service::filter(const std::function<bool(const Subject& subject)>& condition) const {
	std::vector<Subject> result;
	/*for(int i = 0; i < this->repo.size(); ++i) {
		if(condition(this->repo[i])) {
			result.push_back(this->repo[i]);
		}
	}*/
	auto all = this->repo.getAll();
	std::copy_if(all.begin(), all.end(), std::back_inserter(result), condition);
	return result;
}

std::vector<Subject> Service::sort(const std::function<bool(const Subject& s1, const Subject& s2)>& condition,
	bool reverse) const {
	std::vector<Subject> result{this->getAll()};
	/*for(int i = 0; i < this->getAll()->size(); ++i) {
		result.push_back(this->getAll()->at(i));
	}*/

	/*for(int i = 0; i < result.size() - 1; ++i) {
		for(int j = i + 1; j < result.size(); ++j) {
			if(condition(result.at(i), result.at(j)) && !reverse) {
				std::swap(result.at(i), result.at(j));
				continue;
			}
			if(!condition(result.at(i), result.at(j)) && reverse) {
				std::swap(result.at(i), result.at(j));
			}
		}
	}*/
	if (reverse == false) {
		std::sort(result.begin(), result.end(), condition);
	}
	else {
		std::sort(result.begin(), result.end(), [condition](const Subject& s1, const Subject& s2) {
			return !condition(s1, s2);
			});
	}

	return result;
}

void Service::addToContract(const string& name) {
	vector<Subject> subjects = this->filter([name](const Subject& s) {
		return s.getName() == name;
		});

	if (subjects.empty())
		throw std::exception("There are no subjects with given name");

	for (const auto& s : subjects) {
		this->contract.add(s);
	}
}

void Service::clearContract() noexcept {
	this->contract.clear();
}

void Service::generateRandomContract(const int& number) {
	if (number <= 0)
		throw std::exception("Number of subjects must be positive");

	if (number > this->size())
		throw std::exception("Number of subjects must be less than total number of subjects");

	this->contract.clear();
	vector<Subject> subjects = this->getAll();


	const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937_64 rng(seed); // ???
	std::shuffle(subjects.begin(), subjects.end(), rng);

	for (int i = 0; i < number; ++i) {
		this->contract.add(subjects.at(i));
	}
}

void Service::exportContract(const string& file_name) const{
	this->contract.exportToFile(file_name);
}

size_t Service::contractSize() const{
	return this->contract.size();
}

std::map<string, DTO> Service::reportByType() const{
	std::map<string, DTO> result{};
	const vector<Subject> subjects = this->getAll();

	for (const auto& s : subjects) {
		const string stype = s.getType();
		if (result[stype].type.empty()) {
			result[stype].type = stype;
		}
		++result[stype];
	}

	return result;
}

void Service::undo() {
	if (this->undo_list.empty()) {
		throw std::exception("Nothing to undo");
	}
	this->undo_list.back()->doUndo();
	this->undo_list.pop_back();
}

void Service::printContract() const {
	std::cout << this->contract;
}


std::ostream& operator<<(std::ostream& out, const Service& serv) {
	out << serv.repo;
	return out;
}
