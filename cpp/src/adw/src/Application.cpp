#include "adw/Application.h"
#include <adwaita.h>

namespace adw {

    Application::Application(AdwApplication* parent) : parent_(parent) {}

    Glib::RefPtr<Application> Application::create(const Glib::ustring& application_id) {
        adw_init();
        auto adw_app = adw_application_new(application_id.c_str(), G_APPLICATION_DEFAULT_FLAGS);
        return std::make_shared<Application>(adw_app);
    }

    int Application::run(int argc, char** argv) const {
        return g_application_run(G_APPLICATION(parent_), argc, argv);
    }

    int Application::run() const {
        return run(0, nullptr);
    }

    Glib::SignalProxy<void()> Application::signal_activate() {
        g_signal_connect(new_tab_button->gobj(), "clicked", G_CALLBACK(on_new_tab_callback), this);
        const auto signal_info = Glib::SignalProxyInfo {
            "activate",
            Application::on_activate,// GCallback callback;
            nullptr //GCallback notify_callback;
        };

        return Glib::SignalProxy<void()>(this, &signal_info);
    }
}
