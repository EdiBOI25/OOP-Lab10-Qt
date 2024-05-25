#pragma once
#include <vector>

using std::vector;

class Observer {
public:
	virtual ~Observer() = default;
	virtual void obsUpdate() = 0;
};

class ObservableObject {
private:
	vector<Observer*> observers;
public:
	// subscribe /  addListener
	void addObserver(Observer* o) {
		observers.push_back(o);
	}

	// removeListener / unsubscribe
	void removeObserver(Observer* o) {
		observers.erase(std::remove(begin(observers), end(observers), o), observers.end());
	}
protected:
	void notify() {
		for(auto obs: observers) {
			obs->obsUpdate();
		}
	}
};