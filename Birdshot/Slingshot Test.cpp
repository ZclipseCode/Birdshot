#include "ftxuiAll.h"
#include <cmath>
#include <memory>
#include <vector>

using namespace ftxui;

int main() {
	int mouse_x = 0;
	int mouse_y = 0;

    auto renderer_line_braille = Renderer([&] {
        auto c = Canvas(100, 100);
        c.DrawText(0, 0, "Several lines (braille)");
        c.DrawPointLine(mouse_x, mouse_y, 80, 10, Color::Red);
        c.DrawPointLine(80, 10, 80, 40, Color::Blue);
        c.DrawPointLine(80, 40, mouse_x, mouse_y, Color::Green);
        return canvas(std::move(c));
        });

    int selected_tab = 12;
    auto tab = Container::Tab(
        {
            renderer_line_braille,
            //renderer_line_block,
            //renderer_circle_braille,
            //renderer_circle_block,
            //renderer_circle_filled_braille,
            //renderer_circle_filled_block,
            //renderer_ellipse_braille,
            //renderer_ellipse_block,
            //renderer_ellipse_filled_braille,
            //renderer_ellipse_filled_block,

            //renderer_plot_1,
            //renderer_plot_2,
            //renderer_plot_3,

            //renderer_text,
        },
        &selected_tab);

    // This capture the last mouse position.
    auto tab_with_mouse = CatchEvent(tab, [&](Event e) {
        if (e.is_mouse()) {
            mouse_x = (e.mouse().x - 1) * 2;
            mouse_y = (e.mouse().y - 1) * 4;
        }
        return false;
        });

    std::vector<std::string> tab_titles = {
        "line (braille)",
        "line (block)",
        "circle (braille)",
        "circle (block)",
        "circle filled (braille)",
        "circle filled (block)",
        "ellipse (braille)",
        "ellipse (block)",
        "ellipse filled (braille)",
        "ellipse filled (block)",
        "plot_1 simple",
        "plot_2 filled",
        "plot_3 3D",
        "text",
    };
    auto tab_toggle = Menu(&tab_titles, &selected_tab);

    auto component = Container::Horizontal({
        tab_with_mouse,
        tab_toggle,
        });

    // Add some separator to decorate the whole component:
    auto component_renderer = Renderer(component, [&] {
        return hbox({
                   tab_with_mouse->Render(),
                   separator(),
                   tab_toggle->Render(),
            }) |
            border;
        });

    auto screen = ScreenInteractive::FitComponent();
    screen.Loop(component_renderer);

    return 0;
}