#pragma once
#include <adwaita.h>
#include <glibmm/refptr.h>
#include <glibmm/signalproxy.h>
#include <glibmm/ustring.h>
#include <gtkmm/application.h>

namespace adw {
    class Application {
        static void on_activate(Application* app) { app->onActivate(); }

        AdwApplication* parent_ = nullptr;

    protected:
        void onActivate() {}

    public:
        explicit Application(AdwApplication* parent);
        ~Application() = default;

        [[nodiscard]] static Glib::RefPtr<Application> create(const Glib::ustring& application_id = {});
        [[nodiscard]] int run(int argc, char** argv) const;
        [[nodiscard]] int run() const;

        Glib::SignalProxy<void()> signal_activate();
    };
}
