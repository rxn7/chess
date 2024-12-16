#include "imgui_layer.hpp"
#include "board_theme.hpp"
#include "fen.hpp"
#include "renderers/board_renderer.hpp"

#include <imgui-SFML.h>
#include <imgui.h>

ImGuiLayer::ImGuiLayer(sf::RenderWindow &window) {
	assert(ImGui::SFML::Init(window));
}

ImGuiLayer::~ImGuiLayer() {
	ImGui::SFML::Shutdown();
}

void ImGuiLayer::handleEvent(sf::RenderWindow &window, const sf::Event &event) {
	ImGui::SFML::ProcessEvent(window, event);
}

void ImGuiLayer::update(sf::RenderWindow &window, sf::Time frameTime) {
	ImGui::SFML::Update(window, frameTime);
}

void ImGuiLayer::render(sf::RenderWindow &window, Board &board, BoardRenderer &boardRenderer) {
	if(ImGui::Begin("Chess")) {
		if(ImGui::Button("Reset")) {
			board.reset();
		}

		static char input[91];
		ImGui::InputText("##feninput", input, sizeof(input));
		if(ImGui::Button("Apply FEN")) {
			FEN::applyFen(board, input);
		}
	}
	ImGui::End();

	if(ImGui::Begin("Theme")) {
		static float lightColor[3] = {DEFAULT_BOARD_THEME.lightColor.r / 255.0f, DEFAULT_BOARD_THEME.lightColor.g / 255.0f, DEFAULT_BOARD_THEME.lightColor.b / 255.0f};
		ImGui::ColorEdit3("Light##theme", lightColor);

		static float darkColor[3] = {DEFAULT_BOARD_THEME.darkColor.r / 255.0f, DEFAULT_BOARD_THEME.darkColor.g / 255.0f, DEFAULT_BOARD_THEME.darkColor.b / 255.0f};
		ImGui::ColorEdit3("Dark##theme", darkColor);

		if(ImGui::Button("Apply")) {
			BoardTheme theme;
			theme.lightColor = sf::Color(lightColor[0] * 255.0f, lightColor[1] * 255.0f, lightColor[2] * 255.0f);
			theme.darkColor = sf::Color(darkColor[0] * 255.0f, darkColor[1] * 255.0f, darkColor[2] * 255.0f);
			boardRenderer.setTheme(theme);
		}
	}
	ImGui::End();

	ImGui::SFML::Render(window);
}
