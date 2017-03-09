#ifndef BURST_H
#define BURST_H

class Burst {
public:
	// Type of Burst Enumeration
	enum Type {
		CPU, IO
	};

	// Constructor
	Burst(Type type, unsigned int length) {
		this->type = type;
		this->length = length;
	}

	unsigned int get_length() {
		return length;
	}

	void decrement_length(int amount) {
		length -= amount;
	}

	Type get_type() {
		return type;
	}

private:
	// Type of Burst
	Type type;

	// Length of Burst
	unsigned int length;
};

#endif
