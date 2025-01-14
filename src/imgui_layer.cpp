#include "imgui_layer.hpp"
#include "board_theme.hpp"
#include "game.hpp"
#include "fen.hpp"
#include <cstdio>

#ifndef NDEBUG
#include <imgui-SFML.h>
#include <imgui.h>
#endif

#ifndef NDEBUG
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

void ImGuiLayer::render(sf::RenderWindow &window, Game &game) {
	if(ImGui::Begin("Chess")) {
		if(ImGui::Button("Reset##board")) {
			game.restart();
		}

		bool flipped = game.getBoardRenderer().isFlipped();
		if(ImGui::Checkbox("Flipped", &flipped)) {
			game.getBoardRenderer().setFlipped(flipped);
		}

		if(ImGui::Checkbox("Auto flip", &game.autoFlip)) {
			game.performAutoFlip();
		}

		if(ImGui::CollapsingHeader("FEN")) {
			std::string fen = FEN::convertToFen(game.getBoard());
			ImGui::Text("fen: %s", fen.c_str());

			static char input[91];
			ImGui::InputText("##feninput", input, sizeof(input));
			if(ImGui::Button("Apply FEN")) {
				FEN::applyFen(game.getBoard(), input);
			}
		}

		if(ImGui::CollapsingHeader("Material balance")) {
			std::uint8_t whiteMaterialValue = game.getBoard().getPlayer(ChessColor::White).materialValue;
			std::uint8_t blackMaterialValue = game.getBoard().getPlayer(ChessColor::Black).materialValue;
			std::int16_t balance = whiteMaterialValue - blackMaterialValue;
			ImGui::Text("White: %u (%c%d)", whiteMaterialValue, balance < 0 ? '-' : '+', std::abs(balance));
			ImGui::Text("Black: %u (%c%d)", blackMaterialValue, balance <= 0 ? '+' : '-', std::abs(balance));
		}

		if(ImGui::CollapsingHeader("Theme")) {
			static float lightColor[3] = {DEFAULT_BOARD_THEME.lightColor.r / 255.0f, DEFAULT_BOARD_THEME.lightColor.g / 255.0f, DEFAULT_BOARD_THEME.lightColor.b / 255.0f};
			ImGui::ColorEdit3("Light##theme", lightColor);

			static float darkColor[3] = {DEFAULT_BOARD_THEME.darkColor.r / 255.0f, DEFAULT_BOARD_THEME.darkColor.g / 255.0f, DEFAULT_BOARD_THEME.darkColor.b / 255.0f};
			ImGui::ColorEdit3("Dark##theme", darkColor);

			if(ImGui::Button("Apply")) {
				BoardTheme theme = DEFAULT_BOARD_THEME;
				theme.lightColor = sf::Color(lightColor[0] * 255.0f, lightColor[1] * 255.0f, lightColor[2] * 255.0f);
				theme.darkColor = sf::Color(darkColor[0] * 255.0f, darkColor[1] * 255.0f, darkColor[2] * 255.0f);
				game.getBoardRenderer().setTheme(theme);
			}

			if(ImGui::Button("Reset##theme")) {
				game.getBoardRenderer().setTheme(DEFAULT_BOARD_THEME);
			}
		}
	}
	ImGui::End();

	if(ImGui::Begin("Debug")) {
		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);

		static bool vsync = true;
		ImGui::Checkbox("VSync", &vsync);
		window.setVerticalSyncEnabled(vsync);
		
		const DebugData &debug = game.getBoard().getDebugData();
		if(ImGui::CollapsingHeader("Legal moves")) {
			ImGui::Text("found: %u", debug.legalMovesCount);
			ImGui::Text("took: %fms", debug.legalMovesCalculationTime.count() * 1000.0f);
		}
	}
	ImGui::End();

	ImGui::SFML::Render(window);
}
#else
ImGuiLayer::ImGuiLayer(sf::RenderWindow &window) {}	
ImGuiLayer::~ImGuiLayer() {}
void ImGuiLayer::handleEvent(sf::RenderWindow &window, const sf::Event &event) {}
void ImGuiLayer::update(sf::RenderWindow &window, sf::Time frameTime) {}
void ImGuiLayer::render(sf::RenderWindow &window, Game &game) {}
#endif
