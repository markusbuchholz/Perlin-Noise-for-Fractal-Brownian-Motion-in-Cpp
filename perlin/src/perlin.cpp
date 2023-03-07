// Markus Buchholz, 2023
// https://rtouti.github.io/graphics/perlin-noise-algorithm

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <tuple>
#include <vector>
#include <math.h>
#include <random>
#include <cmath>

//---------------------------------------------------------------
// permutation from  https://en.wikipedia.org/wiki/Perlin_noise
// or
// https://cs.nyu.edu/~perlin/noise/

std::vector<int> P = {151, 160, 137, 91, 90, 15,
					  131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
					  190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
					  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
					  77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
					  102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
					  135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
					  5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
					  223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
					  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
					  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
					  49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
					  138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};
//---------------------------------------------------------------

struct Pos
{

	double x;
	double y;
};

//---------------------------------------------------------------

struct Noise
{

	ImVec2 p0;
	ImVec2 p1;
	int color;

	Noise(ImVec2 p0i, ImVec2 p1i, int c) : p0(p0i), p1(p1i), color(c){};
};

//---------------------------------------------------------------

int generateRandom()
{

	std::random_device engine;
	std::mt19937 generator(engine());
	std::uniform_int_distribution<int> distrib(0, 255);
	return distrib(generator);
}
//---------------------------------------------------------------

Pos evaluateCorners(int v)
{
	auto h = v % 4;
	if (h == 0)
	{
		return {1.0, 1.0};
	}
	else if (h == 1)
	{
		return {-1.0, 1.0};
	}
	else if (h == 2)
	{
		return {-1.0, -1.0};
	}
	else
	{
		return {1.0, -1.0};
	}
}

//---------------------------------------------------------------

double dotProduct(Pos a, Pos b)
{

	return {a.x * b.x + a.y * b.y};
}

float Noise2D(float x, float y)
{

	int Xi = std::floor(x);
	int X = Xi % 255;
	int Yi = std::floor(y);
	int Y = Yi % 255;

	float xf = x - std::floor(x);
	float yf = y - std::floor(y);

	auto lerp = [=](float t, float a, float b)
	{
		return a + t * (b - a);
	};

	auto blend = [=](float t)

	{
		return 6 * std::pow(t, 5) - 15 * std::pow(t, 4) + 10 * std::pow(t, 3);
	};

	float valueTopRight = P[P[X + 1] + Y + 1];
	float valueTopLeft = P[P[X] + Y + 1];
	float valueBottomRight = P[P[X + 1] + Y];
	float valueBottomLeft = P[P[X] + Y];

	Pos topRight = {xf - 1.0, yf - 1.0};
	Pos topLeft = {xf, yf - 1.0};
	Pos bottomRight = {xf - 1.0, yf};
	Pos bottomLeft = {xf, yf};

	float gradientTopRight = dotProduct(topRight, evaluateCorners(valueTopRight));
	float gradientTopLeft = dotProduct(topLeft, evaluateCorners(valueTopLeft));
	float gradientBottomRight = dotProduct(bottomRight, evaluateCorners(valueBottomRight));
	float gradientBottomLeft = dotProduct(bottomLeft, evaluateCorners(valueBottomLeft));

	float u = blend(xf);
	float v = blend(yf);

	float f1 = lerp(v, gradientBottomLeft, gradientTopLeft);
	float f2 = lerp(v, gradientBottomRight, gradientTopRight);

	float f3 = lerp(u, f1, f2);

	return f3;
}
//---------------------------------------------------------------

std::vector<std::tuple<float, float, float>> perlinNoise(float fx)
{

	std::vector<std::tuple<float, float, float>> rgb;

	for (float y = 0.0; y < 1000; y++)
	{

		for (float x = 0.0; x < 1000; x++)
		{
			float n = 0.0;
			float a = 1.0;
			float H = 0.5;
			float G = std::exp2(-H);

			float f = fx; // value taken from ImGui
			for (int oct = 0; oct < 8; oct++)
			{
				auto v = a * Noise2D(x * f, y * f);
				n += v;

				f *= 2.0;
				a *= G;
			}

			n += 1.0;
			n *= 0.5;
			int color = std::round(255 * n);
			if (n < 0.5)
			{
				rgb.push_back(std::make_tuple(0.0, 0.0, (color * 2)));
			}
			else if (n < 0.9)
			{
				rgb.push_back(std::make_tuple(0.0, color, std::round(color * 0.5)));
			}
			else
			{
				rgb.push_back(std::make_tuple(color, color, color));
			}
		}
	}

	return rgb;
}

//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	int w = 1000;
	int h = 1000;

	int size = 1;
	int rows = 1000; // w / size;
	int cols = 1000; // h / size;

	std::string title = "Fourier series";
	initImgui(w, h, title);

	// simulation state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 ray_color2 = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

	// Main loop
	int ii = 0;
	bool flag = true;
	std::list<ImVec2> wave_tail;
	std::list<int> test;
	static float fx = 0.1;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		auto colorNoise2D = perlinNoise(fx);
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		std::vector<Noise> noise_img;

		for (int ii = 0; ii < cols; ii++)
		{
			for (int jj = 0; jj < rows; jj++)
			{

				Noise n({(float)ii * size, (float)jj * size}, {(float)ii * size + size, (float)jj * size + 2 * size}, 255);
				noise_img.emplace_back(n);
			}
		}
		int jj = 0;
		for (auto &ii : noise_img)
		{
			draw_list->AddRectFilled(ii.p0, ii.p1, IM_COL32(std::get<0>(colorNoise2D[jj]), std::get<1>(colorNoise2D[jj]), std::get<2>(colorNoise2D[jj]), 255));
			jj++;
		}

		ImGui::PushItemWidth(300);
		ImGui::SliderFloat("Frequency", &fx, 0.001, 0.1);
		ImGui::PopItemWidth();
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
