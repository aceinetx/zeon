#include "zeon.hh"

void z::Zeon::Draw() {
  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("zeon", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoMove);
  ImGui::Text("zeon");

  ImGui::End();
}
