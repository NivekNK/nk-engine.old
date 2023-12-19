#include <nk/entry_point.h>

class Editor : public nk::App {
public:
    Editor() : nk::App({
        .name = "Editor",
        .start_pos_x = 100,
        .start_pos_y = 100,
        .start_width = 1280,
        .start_height = 720
    }) {
        DebugLog("Create Editor");
    }

    virtual ~Editor() override {
        DebugLog("Editor destroyed");
    }
};

CREATE_APP(Editor)
