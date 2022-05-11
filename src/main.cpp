//#include <unistd.h>
#include <iostream>
#include <sys/types.h>
//#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <utility>
#include "PthreadData.cpp"
#include <vector>
#include <ctime>
void* thread_func(void* data) {
	static_cast<PthreadData*>(data)->calculate();
	return NULL;
}
void print_matr(matrix& m) {
	for(unsigned int i = 0; i < m.size(); i++) {
		for(unsigned int j = 0; j < m[0].size(); j++)
			std::cout << m[i][j] << '\t';
		std::cout << std::endl;
	}
}

using matrix = std::vector<std::vector<double>>;
int main() {
	unsigned int start_time = clock();
	unsigned int num_of_threads;
	unsigned int pixel_matr_height, pixel_matr_width;
	unsigned int convolution_matr_side; // %2=1
	//double* pixel_matr = new double [pixel_matr_height*pixel_matr_width];
	//double copy_pixel_matr[pixel_matr_height][pixel_matr_width] = { 0 };
	//double convolution_matr[convolution_matr_side][convolution_matr_side];

	std::cin >> pixel_matr_height >> pixel_matr_width;
	matrix pixel_matr(pixel_matr_height, std::vector<double>(pixel_matr_width, 0));
	matrix copy_pixel_matr = pixel_matr; //strange, when I put this declaration above size input, there where no compile-errors, but abortion
	//before the start of the prog
	for(unsigned int i = 0; i < pixel_matr_height; i++)
		for(unsigned int j = 0; j < pixel_matr_width; j++)
			std::cin >> pixel_matr[i][j];

	std::cin >> convolution_matr_side;
	matrix convolution_matr(convolution_matr_side, std::vector<double>(convolution_matr_side, 0));
	for(unsigned int i = 0; i < convolution_matr_side; i++)
		for(unsigned int j = 0; j < convolution_matr_side; j++)
			std::cin >> convolution_matr[i][j];
	std::cin >> num_of_threads;


	std::pair<int,int> init_current_pixel(std::pair<int,int>(0,0));
	PthreadData init_unit(pixel_matr, copy_pixel_matr, convolution_matr, init_current_pixel);
	std::vector<pthread_t> threads(num_of_threads); //arr i[b] -- intresting, this array in dynamic memory, should I free() it? why I can make it?
	std::vector<PthreadData> pthreaddata(num_of_threads, init_unit);
	const unsigned int&& num_of_pixels = pixel_matr_height * pixel_matr_width;
	if(num_of_threads > num_of_pixels) num_of_threads = num_of_pixels;

	const unsigned int num_of_execution = 3;
	/*std::cout << "original:" << std::endl;
	print_matr(pixel_matr);
	std::cout << "copy:" << std::endl;
	print_matr(copy_pixel_matr);*/

	for(unsigned int i = 0; i < num_of_execution; i++) {
		unsigned int pixels_done(0); //i don't undersrtand how does it work? initialising in loop
		while(pixels_done < num_of_pixels) {
			unsigned int threads_to_open = std::min(num_of_threads, num_of_pixels-pixels_done);
			for(unsigned int j = 0; j < threads_to_open; j++) {
				int&& current_pixel = (pixels_done + 1) - 1;
				pthreaddata[j].current_pixel = std::pair<int,int>(current_pixel/pixel_matr_width, current_pixel%pixel_matr_width);
				++pixels_done;
				pthread_create(&(threads[j]), NULL, thread_func, &(pthreaddata[j]));
			}
			for(unsigned int j = 0; j < threads_to_open; j++) {
				pthread_join(threads[j], NULL); // if they will complete unconsistanly, and I will wait for 1, but everyone
								//has already finished?
			}
			/*std::cout << "original:" << std::endl;
			print_matr(pixel_matr);
			std::cout << "copy:" << std::endl;
			print_matr(copy_pixel_matr);*/
		}
		/*for(unsigned int j = 0; j < num_of_threads; j++) {
			std::swap(pthreaddata[j].pixel_matr, pthreaddata[j].copy_pixel_matr);
		}*/
		std::swap(pixel_matr, copy_pixel_matr); //if I swap in main, will objects, that have this units as reference seap?
		pixels_done = 0;
	}
	/*matrix* output_matr(NULL); //can I do that with & or &&?
	if(num_of_execution%2 == 0) output_matr = &pixel_matr;
	else output_matr = &copy_pixel_matr;
	print_matr(*output_matr);*/
	print_matr(pixel_matr);
	unsigned int end_time = clock();
	std::cout << end_time - start_time << std::endl;
}

