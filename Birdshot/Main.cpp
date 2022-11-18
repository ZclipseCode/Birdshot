#include "ftxuiAll.h"
#include <string>

using namespace ftxui;

int main() {
	//Input Name (User Input via Console)
    std::string name;
    Component inputName = Input(&name, "Input Name Here...");
    auto component = Container::Vertical({
        inputName
    });

    //If Name is Greater than Three Characters, Try Again (to Capture Arcade-Like Feel) (Input Validation)
    std::string nameValidation;

    //renderer is like Unity Update()
    auto renderer = Renderer(component, [&] {
        if (name.length() == 0) {
            nameValidation = "";
        }
        else if (name.length() < 3) {
            nameValidation = "Name is too short!";
        }
        else if (name.length() > 3) {
            nameValidation = "Name is too long!";
        }
        else {
            for (int i = 0; i < name.length(); i++) {
                name[i] = std::toupper(name[i]);
            }
            nameValidation = "Welcome, " + name + "!";
        }

        return vbox({
            text("   ___  _        __    __        __ "),
            text("  / _ )(_)______/ /__ / /  ___  / /_"),
            text(" / _  / / __/ _  (_-</ _ \\/ _ \\/ __/"),
            text("/____/_/_/  \\___/___/_//_/\\___/\\__/ "),
            text(""),
            text("Created by Brian Michael"),
            text(""),
            text(nameValidation),
            inputName->Render()
        });
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

	return 0;
}