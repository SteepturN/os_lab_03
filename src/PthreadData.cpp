#include <utility>
#include <vector>
using matrix = std::vector<std::vector<double>>;
struct PthreadData {
	PthreadData(matrix& pixel_matr, matrix& copy_pixel_matr, matrix& convolution_matr, std::pair<int, int>& current_pixel)
	: pixel_matr(pixel_matr), copy_pixel_matr(copy_pixel_matr), convolution_matr(convolution_matr), current_pixel(current_pixel), \
	pixel_matr_height(pixel_matr.size()), pixel_matr_width(pixel_matr[0].size()),\
	convolution_matr_side(convolution_matr.size()) {}
	matrix& pixel_matr;
	matrix& copy_pixel_matr;
	matrix& convolution_matr;
	std::pair<int, int> current_pixel;
	const int pixel_matr_height; //can't use &&, why?
	const int pixel_matr_width;
	const int convolution_matr_side;
	void calculate() {
		double sum(0);
		std::pair<int, int> cur = std::pair<int,int>(current_pixel.first-convolution_matr_side/2,	\
								current_pixel.second-convolution_matr_side/2);
		for(int cur_conv_y = 0, cur_pixel_y = cur.first; cur_conv_y < convolution_matr_side; cur_conv_y++, cur_pixel_y++) {
			for(int cur_conv_x = 0, cur_pixel_x = cur.second; cur_conv_x < convolution_matr_side; cur_conv_x++, cur_pixel_x++) {
				int koef_x(0), koef_y(0);
				if(cur_pixel_x < 0) koef_x = -cur_pixel_x;
				else if(cur_pixel_x > static_cast<int>(pixel_matr_width-1)) koef_x = -cur_pixel_x + pixel_matr_width-1;
				if(cur_pixel_y < 0) koef_y = -cur_pixel_y;
				else if(cur_pixel_y > static_cast<int>(pixel_matr_height-1)) koef_y = -cur_pixel_y + pixel_matr_height-1;
				sum += pixel_matr[cur_pixel_y + koef_y][cur_pixel_x + koef_x] * convolution_matr[cur_conv_y][cur_conv_x];
			}
		}
		copy_pixel_matr[current_pixel.first][current_pixel.second] = sum;
	}
};
