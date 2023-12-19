#include <nk/entry_point.h>

class Editor : public nk::App {
public:
    Editor() : nk::App("Editor") {
        DebugLog("Create Editor");
    }

    virtual ~Editor() override {
        DebugLog("Editor destroyed");
    }
};

CREATE_APP(Editor)
