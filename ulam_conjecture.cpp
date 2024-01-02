#include <iostream>
#include <vector>

void run(long long int x, std::vector<int>& elements){
	elements.push_back(x);
	if (x == 1){
		return ;
	}
	if (x%2){
		run(3*x+1, elements);
	}
	else{
		run(x/2, elements);
	}
}

int main(int argc, char **args){

	std::vector<int> elements;
	run(std::stoi(args[1]), elements);

	for (int r: elements){
		std::cout << r << std::endl;
	}

	return 0;
}


