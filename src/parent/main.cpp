#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <cstdlib>
#include <charconv>
enum {
	READ_END=0,
	WRITE_END=1,
};
int main() {
	char filename[20];
	std::cin >> filename;
	int number(1); //>0
	bool all_ok = true;

	int pipe_parent_to_son[2];//
	char pipe_son_reading[20] = { 0 };

	int pipe_son_to_parent[2];
	char pipe_son_writing[20] = { 0 };

	if(pipe(pipe_parent_to_son)==-1) {//
		std::cerr << "can't create pipe\n";//
		return 1;//
	}
	if(pipe(pipe_son_to_parent)==-1) {//
		std::cerr << "can't create pipe\n";//
		return 1;//
	}
	if(std::to_chars(pipe_son_reading, pipe_son_reading+20, pipe_parent_to_son[READ_END]).ec == std::errc::value_too_large) {
		std::cerr << "num of descriptor is too large\n";
		return -1;
	}
	//locale-independent - what does it mean?
	if(std::to_chars(pipe_son_writing, pipe_son_writing+20, pipe_son_to_parent[WRITE_END]).ec == std::errc::value_too_large) {
		std::cerr << "num of descriptor is too large\n";
		return -1;
	}
	//std::cout <<pipe_parent_to_son[READ_END] << " = " << pipe_son_reading << ' ' << pipe_son_to_parent[WRITE_END] << " = " << pipe_son_writing << std::endl;
	pid_t pid = fork();
	if(pid < 0) { //
		std::cerr << "can' create another process\n";//
		return 2;//
	} else if(pid == 0) {//
		//std::cout << "from child: child pid:" << getpid() << std::endl;//
		if(execl(filename, pipe_son_reading, pipe_son_writing, static_cast<char*>(NULL)) == -1) {// if I give pointer, would it work?
			std::cerr << "can't execute prog " << filename << std::endl;
			return 3;
		}
	} else {
		//std::cout << "from parent: child pid" << pid << std::endl;
	}
	//FILE write_nums_here = fdopen(pipe_parent_to_son[1], "w"); - it's wrong but why?
	while(all_ok) {
		std::cin >> number;
		if(write(pipe_parent_to_son[WRITE_END], &number, sizeof(number)) == -1) {
			std::cerr << "parent: error writing to pipe";
			all_ok = false;
		} else if(read(pipe_son_to_parent[READ_END], &all_ok, sizeof(all_ok)) == -1) {
			std::cerr << "parent: error reading from pipe";
			all_ok = false;
		}
	}
	return 0;
}



