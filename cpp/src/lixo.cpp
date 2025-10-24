#include <gtkmm.h>
#include <adwaita.h>
#include <memory>
#include <iostream>
#include <glibmm.h>
#include <sigc++-3.0/sigc++/sigc++.h>

#include "adw/include/adw/Application.h"

class MyApplication : public Gio::Application {
private:
    /** This is just a way to call Glib::init() before calling a Glib::Object ctor,
     * so that glibmm's GQuarks are created before they are used.
     */
    const Glib::Class& custom_class_init();
protected:
  explicit MyApplication(
      const Glib::ustring& application_id = {},
      Gio::Application::Flags flags = Gio::Application::Flags::NONE);
public:
    // noncopyable
    MyApplication(const Application&) = delete;
    MyApplication& operator=(const Application&) = delete;

    explicit MyApplication(const Glib::SignalProxy<void()>& signal_activate)
        : ObjectBase("MyApplication")
    {}

    static Glib::RefPtr<MyApplication> create(
        const Glib::ustring& application_id = {},
        Gio::Application::Flags flags = Gio::Application::Flags::NONE);
};

namespace
{
    // Make sure the C++ locale is the same as the C locale.
    // The first call to Glib::init() fixes this. We repeat it here just to be sure.
    // The application program may have changed the locale settings after
    // the first call to Glib::init(). Unlikely, but possible.
    static void set_cxx_locale_to_c_locale()
    {
        try
        {
            // Make the C++ locale equal to the C locale.
            std::locale::global(std::locale(std::setlocale(LC_ALL, nullptr)));
        }
        catch (const std::runtime_error& ex)
        {
            g_warning("Can't make the global C++ locale equal to the C locale.\n"
              "   %s\n   C locale = %s\n", ex.what(), std::setlocale(LC_ALL, nullptr));
        }
    }
}

MyApplication::MyApplication(const Glib::ustring& application_id, Gio::Application::Flags flags)
:
  // Mark this class as non-derived to allow C++ vfuncs to be skipped.
  // GApplication complains about "" but allows nullptr, so we avoid passing "".
  Glib::ObjectBase(nullptr),
  Gio::Application(Glib::ConstructParams(custom_class_init(),
    "application_id", Glib::c_str_or_nullptr(application_id),
    "flags", static_cast<GApplicationFlags>(flags), nullptr))
{
    // gtk_init() is called by the 'startup' default signal handler when g_application_run() is called.
    // It's also called here, to make it possible for users of gtkmm to create
    // a window and other widgets before calling run(). (This is not recommended.)
    // See https://bugzilla.gnome.org/show_bug.cgi?id=639925
    adw_init();
    set_cxx_locale_to_c_locale();
}

Glib::RefPtr<MyApplication> MyApplication::create(
    const Glib::ustring& application_id,
    Gio::Application::Flags flags
) {
    return Glib::RefPtr<MyApplication>( new MyApplication(application_id, flags) );
}

int main()
{
    Glib::RefPtr<MyApplication> app = MyApplication::create("nautix");
    app->signal_activate().connect([app] {
        // Create and show the window when the application activates
        //auto window = Gtk::make_managed<MainWindow>();
        //app->add_window(*window); // Add the window to the application
        //window->present();
    });
}














class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow() {
        set_title("Adwaita Styling with gtkmm");
        set_default_size(800, 600);
    }
};

int main2(int argc, char* argv[]) {
    int result;
/**/
    adw_init();
    auto gtkapp = Gtk::Application::create("org.nautix.app");

    //g_application_activate
    // ATT: New application windows must be added after the GApplication::startup signal has been emitted.
    // Connect the activate signal
    sigc::slot<void()> x;
    gtkapp->signal_activate().connect([gtkapp]() {
        // Create and show the window when the application activates
        auto window = Gtk::make_managed<Gtk::Window>();
        gtkapp->add_window(*window); // Add the window to the application
        window->present();
    });
    result = gtkapp->run(argc, argv);
/**
    auto app = adw::Application::create("org.nautix.app");
    app->signal_activate().connect([gtkapp]() {
        // Create and show the window when the application activates
        auto window = Gtk::make_managed<MainWindow>();
        //app->add_window(*window); // Add the window to the application
        window->present();
    });
    result = app->run(argc, argv);
**/
    return result;
}

