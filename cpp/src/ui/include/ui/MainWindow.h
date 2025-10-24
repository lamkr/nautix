#pragma once

#include <adwaita.h>
#include <gtkmm.h>

namespace nautix::ui {

    class MainWindow final : public Gtk::ApplicationWindow {
        static void on_new_tab_callback(Gtk::Button& button, gpointer user_data);

        // Membros da classe para manter o estado
        AdwTabView* tab_view_ = nullptr;
        int tab_count_ = 0;

        void initialize_window();
        void build_content();
        void build_content_();
        Gtk::HeaderBar* create_header_bar();
        void add_new_tab_button(Gtk::HeaderBar* header_bar);

    public:
        explicit MainWindow();

        void on_new_tab();
    };

}
