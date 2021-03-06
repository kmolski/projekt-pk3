module;

import libtui_base;

#include <cmath>
#include <iostream>
#include <unordered_map>

export module libtui_button;

export class Button : public Widget {
    std::string label;
    std::unordered_map<std::string, std::function<ActionResult(Button *)>> actions;

  public:
    Button(Position _pos, Size _size, Color _color, std::string _label)
        : Widget(_pos, _size, true, _color), label(_label) {
        actions = {{"SPACE", [](Button *) {
                        std::cerr << "Button pressed.\r\n";
                        return ActionResult::Success;
                    }}};
    }

    void draw() {
        Size screen_size = get_term_size();

        use_color();

        if (active) {
            std::cout << CSI << "1m" << CSI
                      << "7m"; // Set bold and inverse color for active buttons
        }

        for (uint x = 0; x < size.height && pos.x + x < screen_size.height + 1; ++x) {
            std::cout << CSI << pos.x + x << ';' << pos.y << 'f';
            for (uint y = 0; y < size.width && pos.y + y < screen_size.width + 1; ++y) {
                std::cout << ' ';
            }
        }

        std::string button_text =
            (label.length() > size.width ? label.substr(0, std::max(0, int(size.width - 4))) + "..."
                                         : label);

        uint label_x = pos.x + std::floor(size.height / 2.0),
             label_y = pos.y + std::round(size.width / 2.0) - std::round(button_text.length() / 2);

        if (size.height > 0 && size.width > 0 && label_x <= screen_size.height &&
            label_y - 1 <= screen_size.width) {
            std::cout << CSI << label_x << ';' << label_y << 'f';
            std::cout << button_text.substr(0, std::max(0, int(screen_size.width - label_y + 1)));
        }

        reset_color();
    }

    ActionResult handle_key(char) { return ActionResult::GetShortcut; }
    ActionResult handle_shortcut(std::string shortcut) {
        if (actions.find(shortcut) != actions.end()) {
            return actions[shortcut](this);
        } else {
            return ActionResult::Failed;
        }
    }

    Actionable *get_active_child() { return nullptr; }
};
