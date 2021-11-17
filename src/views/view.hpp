#pragma once

class View {
    const char* name_;

protected:
    void show_view_info();

public:
    View(const char* name);
    virtual ~View() = default;

    [[nodiscard]] const char* name() const { return name_; }

    virtual void render_content();
    virtual void render_ui();
};
