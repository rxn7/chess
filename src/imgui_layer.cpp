#include "imgui_layer.hpp"

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

void ImGuiLayer::render(sf::RenderWindow &window, Board &board) {
        if(ImGui::Begin("Chess")) {
                static char input[91];
                ImGui::InputText("##feninput", input, sizeof(input));

                if(ImGui::Button("Apply FEN")) {
                        board.applyFen(input);
                }
        }

        ImGui::End();
        ImGui::SFML::Render(window);
}