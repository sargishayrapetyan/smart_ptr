#include "shared_ptr.h"

#include <iostream>

template<typename T>
void use_count(const smart_ptr::shared_ptr<T>& ptr)
{
	std::cout << ptr.use_count() << std::endl;
}

template<typename T>
void check_validity(const smart_ptr::shared_ptr<T>& ptr)
{
	if (ptr) {
		std::cout << "Valid pointer by bool operator\n";
	} else {
		std::cout << "Invalid pointer by bool operator\n";
	}
	if (ptr.get()) {
		std::cout << "Valid pointer by get\n";
	} else {
		std::cout << "Invalid pointer by get\n";
	}
}

template<typename T>
void check_value(const smart_ptr::shared_ptr<T>& ptr) 
{
	std::cout << "Value is " << *ptr << std::endl;
}

int main()
{
	smart_ptr::shared_ptr<int> p1;
	p1.reset();
	std::cout << "=====TEST null shared pointer=====\n";
	use_count(p1);
	check_validity(p1);
	//check_value(p1); should crash
	std::cout << "\n";

	smart_ptr::shared_ptr<int> p2(p1);
	std::cout << "=====TEST simple shared_ptr=====\n";
	use_count(p2);
	check_validity(p2);
	// check_value(p2); should crash
	std::cout << "\n";

	smart_ptr::shared_ptr<std::string> p3(new std::string("BOBO String"));
	std::cout << "=====TEST string shared_ptr=====\n";
	use_count(p3);
	check_validity(p3);
	check_value(p3);
	std::cout << "String size is " << p3->size() << std::endl;
	p3.reset(new std::string("BO string"));
	std::cout << "new String size is " << p3->size() << '\n';

	smart_ptr::shared_ptr<std::string> p4(p3);
	std::cout << "=====TEST copy=====\n";
	use_count(p4);
	check_validity(p4);
	check_value(p4);
	std::cout << "String size is " << p4->size() << std::endl;
	if (p3 == p4) {
		std::cout << "Equality passed\n";
	} else {
		std::cout << "Equality failed\n";
	}
	std::cout << "\n";

	smart_ptr::shared_ptr<std::string> p5(std::move(p3));
	std::cout << "=====TEST move=====\n";
	use_count(p5);
	check_validity(p5);
	check_value(p5);
	std::cout << "String size is " << p5->size() << std::endl;
	if (p5 == p4) {
		std::cout << "Equality passed\n";
	} else {
		std::cout << "Equality failed\n";
	}
	
	if (!p3) {
		std::cout << "Move c_tor passed\n";
	} else {
		std::cout << "Move c_tor failed\n";
	}
	std::cout << "\n";

	smart_ptr::shared_ptr<std::string> p6 = p5;
	std::cout << "=====TEST assignment operator=====\n";
	use_count(p6);
	check_validity(p6);
	check_value(p6);
	std::cout << "String size is " << p6->size() << std::endl;
	if (p5 == p6) {
		std::cout << "Equality passed\n";
	} else {
		std::cout << "Equality failed\n";
	}
	std::cout << "\n";

	{
		smart_ptr::shared_ptr<std::string> p7(new std::string("Urish string"));
		p7.swap(p6);
		std::cout << "=====TEST swapping=====\n";
		use_count(p7);
		use_count(p6);
		check_value(p7);
		check_value(p6);
		if (p7 != p6) {
			std::cout << "Equality passed\n";
		} else {
			std::cout << "Equality failed\n";
		}
		std::cout << "\n";
	}
	use_count(p5);

	return 0;
}
