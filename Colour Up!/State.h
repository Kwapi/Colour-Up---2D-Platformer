#pragma once


class State {

public:
	
	class Context {
	public:
		Context(int& sw, int& sh) : screenWidth(&sw), screenHeight(&sh) { }

		int *screenWidth;
		int *screenHeight;
		//ResourceHolder*
	};

	State();
	~State();

	Context& getContext();



	virtual void processKeys(bool *keySet) = 0;
	virtual void update(const double dt) = 0;
	virtual void render() = 0;


};

