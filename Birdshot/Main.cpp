#include "ftxuiAll.h"
#include <string>
#include <vector>

#include <cmath>
#include <utility>
#include <memory>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    //Button to Transition to Play Screen after Name is Selected
    bool playActive = false;
    auto playButton = Container::Horizontal({
        Button("Play!", [&] {playActive = true; })
    });

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
        if (name.length() != 3) {
            return vbox({
            text(nameValidation) | hcenter,
            inputName->Render() | hcenter,
            });
        }
        else {
            return vbox({
            text(nameValidation) | hcenter,
            inputName->Render() | hcenter,
            playButton->Render() | hcenter
            });
        }
        });

    //play tab
    int mouse_x = 0;

    auto playTabRenderer = Renderer([&] {
        auto c = Canvas(100, 1000);
        c.DrawPointLine(40, 40, mouse_x, 0);
        c.DrawPointLine(40, 40, 60, 40);
        c.DrawPointLine(60, 40, mouse_x, 0);
        return canvas(std::move(c)) | border;
        });

    //tabs
    int tab_index = 0;
    std::vector<std::string> tab_entries = {
        " Input Name ", " Play ", " Leaderboard ", " How to Play ", " Settings "
    };
    auto tab_selection =
        Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated()) | border | hcenter;
    auto tab_content = Container::Tab(
        {
            inputNameRenderer | hcenter,
            playTabRenderer | hcenter
        },
        &tab_index);

    //mouse
    auto tab_with_mouse = CatchEvent(tab_content, [&](Event e) {
        if (e.is_mouse()) {
            mouse_x = e.mouse().x;
        }
        return false;
        });

    //used for everything?
    auto main_container = Container::Vertical({
        tab_selection,
        tab_content,
        tab_with_mouse
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