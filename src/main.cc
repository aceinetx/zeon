#include "zeon.hh"

int main() {
  z::Zeon zeon;
  zeon.Init();
  zeon.Run();
  zeon.Cleanup();
}
