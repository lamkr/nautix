#pragma once

#include <adwaita.h>
#include <gtkmm.h>
#include "ui/ContentView.h"
#include "ui/NavigationPanel.h"
#include "ui/StatusBar.h"

namespace nautix::ui {

    class MainWindow final : public Gtk::ApplicationWindow {
        AdwTabView* tabView_;
        NavigationPanel* navigationPanel_;
        ContentView* contentView_;
        StatusBar* statusBar_;
        // Core UI Components for the new MVP
        Gtk::Paned m_main_paned;          // Resizable container

        Glib::RefPtr<Gtk::Builder> m_refBuilder;
    public:
        explicit MainWindow();
    };

} // namespace nautix::ui
