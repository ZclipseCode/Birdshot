// For personal use to show unicode
// Copy and paste into Windows Terminal:
// & C:\Users\bmich\source\repos\Birdshot\Debug\Birdshot.exe

#include "ftxuiAll.h"
#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "projectile.h"
#include "rock.h"
#include "laser.h"
#include "remoteControlledRock.h"

using namespace ftxui;

//Different Bird Types (Speeds) (Enumerated Type (enum) (User-Defined Data Types)
enum class birdSpeed {
    slow,
    medium,
    fast
};

int BirdSpeedAssign(birdSpeed bs) {
    switch (bs) {
    case birdSpeed::slow:
        return 1;
        break;
    case birdSpeed::medium:
        return 2;
        break;
    case birdSpeed::fast:
        return 3;
        break;
    default:
        return 1;
        break;
    }
}

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
        if (name.length() != 3) {
            return vbox({
            text(nameValidation) | hcenter,
            inputName->Render() | hcenter,
            });
        }
        else {
            return vbox({
            text(nameValidation) | hcenter,
            inputName->Render() | hcenter
            //playButton->Render() | hcenter
            });
        }
        });

    //settings
    const std::vector<std::string> projectileEntries = {
        "Rock",
        "Laser",
        "Remote-Controlled Rock"
    };
    int projectileSelected = 0;
    Component projectileSel = Radiobox(&projectileEntries, &projectileSelected);

    const std::vector<std::string> speedEntries = {
        "Slow",
        "Medium",
        "Fast"
    };
    int speedSelected = 1;
    Component speedSel = Radiobox(&speedEntries, &speedSelected);

    auto compilerComponent = Container::Horizontal({
        projectileSel,
        speedSel
        });

    auto settingsCommand = [&] {
        Elements line;
        line.push_back(text(projectileEntries[projectileSelected]) | bold);
        line.push_back(text(speedEntries[speedSelected]) | bold);
        return line;
    };

    //settings tab
    rock* mainRock = new rock;
    laser* mainLaser = new laser;
    remoteControlledRock* mainRcr = new remoteControlledRock;

    bool rockOn = true;
    bool laserOn = false;
    bool rcrOn = false;

    birdSpeed currentSlow = birdSpeed::slow;
    birdSpeed currentMedium = birdSpeed::medium;
    birdSpeed currentFast = birdSpeed::fast;

    int currentSpeed = BirdSpeedAssign(currentSlow);

    auto settingsRenderer = Renderer(compilerComponent, [&] {
        auto projectileWindow = window(text("Projectile"),
            projectileSel->Render() | frame);

        if (projectileSelected == 0) {
            mainRock->Activate(rockOn, laserOn, rcrOn);
        }
        else if (projectileSelected == 1) {
            mainLaser->Activate(rockOn, laserOn, rcrOn);
        }
        else if (projectileSelected == 2) {
            mainRcr->Activate(rockOn, laserOn, rcrOn);
        }

        auto speedWindow = window(text("Bird Speed"),
            speedSel->Render() | frame);

        if (speedSelected == 0) {
            currentSpeed = BirdSpeedAssign(currentSlow);
        }
        else if (speedSelected == 1) {
            currentSpeed = BirdSpeedAssign(currentMedium);
        }
        else if (speedSelected == 2) {
            currentSpeed = BirdSpeedAssign(currentFast);
        }

        return vbox({
            vbox({
                projectileWindow,
                speedWindow
                })
            });
        });

    //play tab
    bool shoot = false;

    int mouse_x = 0;
    int mouse_y = 0;

    int laserTravelStart = 100;
    int laserTravelEnd = 80;

    int rockTravelStart = 100;
    int rockAngle = 50;
    bool rockLeft = true;
    bool rockPathed = false;

    int birdX = 108;
    int birdY = 0;

    //Hitting Birds Give Points that are Added to the Total Score (Arithmetic Operations)
    int score = 0;

    auto playTabRenderer = Renderer([&] {
        auto c = Canvas(100, 148); //max Y is 148
        c.DrawPointLine(40, 100, mouse_x - 55, 140);
        c.DrawPointLine(40, 100, 60, 100);
        c.DrawPointLine(60, 100, mouse_x - 55, 140);

        //bird
        std::srand(std::time(0));
        if (birdX >= 108) {
            birdX = 0;
            birdY = std::rand() % (81 - 2) + 2;
        }
        c.DrawPointEllipseFilled(birdX += currentSpeed, birdY, 8, 2);

        if (shoot && rockOn) {
            if (!rockPathed) {
                if (mouse_x - 55 <= 50) {
                    rockLeft = true;
                    rockPathed = true;
                }
                else if (mouse_x - 55 > 50) {
                    rockLeft = false;
                    rockPathed = true;
                }
            }
            
            if (rockLeft) {
                c.DrawPointCircleFilled(rockAngle++, rockTravelStart--, 6);

                if (rockAngle >= 104 || rockTravelStart <= -4) {
                    shoot = false;
                    rockTravelStart = 100;
                    rockAngle = 50;
                    rockPathed = false;
                }
            }
            else if (!rockLeft) {
                c.DrawPointCircleFilled(rockAngle--, rockTravelStart--, 6);

                if (rockAngle <= -4 || rockTravelStart <= -4) {
                    shoot = false;
                    rockTravelStart = 100;
                    rockAngle = 50;
                    rockPathed = false;
                }
            }
        }
        else if (shoot && laserOn) {
            c.DrawPointLine(50, laserTravelStart-= 3, 50, laserTravelEnd-= 3);

            if (laserTravelStart <= 0) {
                shoot = false;
                laserTravelStart = 100;
                laserTravelEnd = 80;
            }
        }
        else if (shoot && rcrOn) {
            if (mouse_x - 55 <= 50) {
                c.DrawPointCircleFilled(rockAngle++, rockTravelStart--, 6);

                if (rockAngle >= 104 || rockTravelStart <= -4) {
                    shoot = false;
                    rockTravelStart = 100;
                    rockAngle = 50;
                }
            }
            else if (mouse_x - 55 > 50) {
                c.DrawPointCircleFilled(rockAngle--, rockTravelStart--, 6);

                if (rockAngle <= -4 || rockTravelStart <= -4) {
                    shoot = false;
                    rockTravelStart = 100;
                    rockAngle = 50;
                }
            }
        }

        //gain score
        c.DrawText(0, 147, "Score: " + std::to_string(score));

        if (rockTravelStart <= birdY + 2 && rockTravelStart >= birdY - 2 && rockAngle <= birdX + 6 && rockAngle >= birdX - 6) {
            score++;
        }

        if (laserTravelStart >= birdY && laserTravelEnd <= birdY && birdX <= 50 + 8 && birdX >= 50 - 8) {
            score++;
        }

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
            playTabRenderer | hcenter,
            settingsRenderer | hcenter
        },
        &tab_index);

    //mouse
    auto tab_with_mouse = CatchEvent(tab_content, [&](Event e) {
        if (e.is_mouse()) {
            mouse_x = e.mouse().x;
            mouse_y = e.mouse().y;

            if (tab_index == 1 && e.mouse().motion && Mouse::Pressed) {
                shoot = true;
            }
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