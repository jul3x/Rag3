//
// Created by jul3x on 16.03.20.
//

#ifndef RAG3_EDITOR_SAVEWINDOW_H
#define RAG3_EDITOR_SAVEWINDOW_H

#include <editor/ChildWindow.h>


namespace editor {

    class SaveWindow : public ChildWindow {
    public:
        SaveWindow(tgui::Gui *gui, tgui::Theme *theme);

    private:
        tgui::Grid::Ptr grid_;

    };

} // namespace editor

#endif //RAG3_EDITOR_SAVEWINDOW_H
