#include "ftxuiAll.h"
#include <string>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();

	//Input Name (User Input via Console)
    std::string name;
    Component inputName = Input(&name, "Input Name Here...");
    auto component = Container::Vertical({
            inputName
        });
    //If Name is Greater than Three Characters, Try Again (to Capture Arcade-Like Feel) (Input Validation)
    std::string nameValidation;
    auto inputNameRenderer = Renderer(component, [&] {
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
            text(nameValidation) | hcenter,
            inputName->Render() | hcenter
            });
        });

    //Button to Transition to Play Screen after Name is Selected
    auto playTabRenderer = Renderer([] {
        return vbox({
            text("game tab")
            });
        });

    //tabs
    int tab_index = 0;
    std::vector<std::string> tab_entries = {
        "Input Name", "Play", "Leaderboard", "Settings"
    };
    auto tab_selection =
        Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
    auto tab_content = Container::Tab(
        {
            inputNameRenderer,
            playTabRenderer
        },
        &tab_index);

    auto main_container = Container::Vertical({
        tab_selection,
        tab_content,
        });

    //renderer is like Unity Update() //but not for everything?
    auto mainRenderer = Renderer(main_container, [&] {
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

    int shift = 0;

    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&] {
        while (refresh_ui_continue) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0.05s);
            screen.Post([&] { shift++; });
            screen.Post(Event::Custom);
        }
        });

    //auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(mainRenderer);
    refresh_ui_continue = false;
    refresh_ui.join();

	return 0;
}