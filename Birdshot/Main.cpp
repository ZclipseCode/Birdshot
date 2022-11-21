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
    auto inputNameRenderer = Renderer([&] {
        return vbox({
            text(nameValidation) | hcenter,
            inputName->Render() | hcenter
            });
        });

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

        //Button to Transition to Play Screen after Name is Selected
        auto gameTabRenderer = Renderer([] {
            return vbox({
                text("game tab")
                });
            });

        int tab_index = 0;
        std::vector<std::string> tab_entries = {
            "Input Name", "Play", "Settings"
        };
        auto tab_selection =
            Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
        auto tab_content = Container::Tab(
            {
                inputNameRenderer,
                gameTabRenderer
            },
            &tab_index);

        auto main_container = Container::Vertical({
            tab_selection,
            tab_content,
            });

        return vbox({
            text("   ___  _        __    __        __ ") | hcenter,
            text("  / _ )(_)______/ /__ / /  ___  / /_") | hcenter,
            text(" / _  / / __/ _  (_-</ _ \\/ _ \\/ __/") | hcenter,
            text("/____/_/_/  \\___/___/_//_/\\___/\\__/ ") | hcenter,
            text("") | hcenter,
            text("Created by Brian Michael") | hcenter,
            text("") | hcenter,

            tab_selection->Render(),
            tab_content->Render() | flex
        });
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

	return 0;
}