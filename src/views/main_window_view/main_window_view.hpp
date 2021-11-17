#pragma once

#include <memory>

#include "views/view.hpp"

class LeftPane;
class RightPane;

class MainWindowView : public View {
    std::unique_ptr<LeftPane> left_pane_;
    std::unique_ptr<RightPane> right_pane_;

public:
    MainWindowView(const char* name);

    void render_ui() override;
};
