#pragma once
#include "repository.h"

class UndoAction {
public:
	virtual void doUndo() = 0;

	virtual ~UndoAction() = default;
};

class UndoAdd: public UndoAction {
private:
	Subject addedSubject;
	AbstractRepo& repo;
public:
	UndoAdd(const Subject& s, AbstractRepo& r) : addedSubject{ s }, repo{ r } {}

	void doUndo() override {
		this->repo.remove(addedSubject);
	}
};

class UndoRemove : public UndoAction {
private:
	Subject removedSubject;
	AbstractRepo& repo;
public:
	UndoRemove(const Subject& s, AbstractRepo& r) : removedSubject{ s }, repo{ r } {}

	void doUndo() override {
		this->repo.add(removedSubject);
	}
};

class UndoUpdate: public UndoAction {
private:
	Subject oldSubject;
	Subject newSubject;
	AbstractRepo& repo;

public:
	UndoUpdate(const Subject& old_s, const Subject& new_s, AbstractRepo& r) : oldSubject{old_s}, newSubject(new_s), repo{r}{}

	void doUndo() override {
		// const int index = this->repo.find(newSubject);
		this->repo.update(newSubject, oldSubject);
	}
};
