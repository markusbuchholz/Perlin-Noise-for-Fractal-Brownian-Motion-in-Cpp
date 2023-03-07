// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <tuple>
#include <vector>
#include <math.h>
#include <random>



//----------- system dynamic parameters --------------------

float dt = M_PI / 720.0;
float P = 0.25;
float scale = 50.0;

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
	std::uniform_int_distribution<int> distrib(1, 255);
	return distrib(generator);
}

//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	int w = 1000;
	int h = 1000;

	int size = 5;
	int rows = 500; 
	int cols = 500;

	std::string title = "Fourier series";
	initImgui(w, h, title);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 ray_color2 = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

	int ii = 0;
	bool flag = true;
	std::list<ImVec2> wave_tail;
	std::list<int> test;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		static int grey_nr = 200;

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

				Noise n({(float)ii * size, (float)jj * size}, {(float)ii * size +size, (float)jj * size + 2* size}, generateRandom());
				noise_img.emplace_back(n);
			}
		}

		for (auto &ii : noise_img)
		{
			draw_list->AddRectFilled(ii.p0, ii.p1, IM_COL32(255, 255, 255, ii.color));
		}


		ImGui::PushItemWidth(200);
		ImGui::SliderInt("Grey intensity", &grey_nr, 1, 255);
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
