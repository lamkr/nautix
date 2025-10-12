#include "ui/StatusBar.h"
namespace nautix::ui {
    StatusBar::StatusBar() {
        add_css_class("flat");
        // Adicionamos uma classe CSS para podermos estilizá-la.
        add_css_class("status-bar");
        auto label = Gtk::make_managed<Gtk::Label>("Status Bar");
        label->set_halign(Gtk::Align::START);
        label->set_valign(Gtk::Align::CENTER);
        label->set_margin_start(12);
        set_child(*label);

        set_vexpand(false);
        set_size_request(-1, 32);
    }
}