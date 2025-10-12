#include "ui/MainWindow.h"

namespace nautix::ui {
    MainWindow::MainWindow() {
        set_title("Nautix");

        maximize();

        tabView_ = adw_tab_view_new();
        // Attach the TabBar to the TabView.
        AdwTabBar* tab_bar = adw_tab_bar_new();
        adw_tab_bar_set_view(tab_bar, tabView_);

        // 2. Cria os painéis de conteúdo da aba (placeholders)
        navigationPanel_ = Gtk::make_managed<NavigationPanel>();
        contentView_ = Gtk::make_managed<ContentView>();
        statusBar_ = Gtk::make_managed<StatusBar>();

        // 3. Cria o divisor redimensionável e adiciona os painéis esquerdo e direito
        auto paned = Gtk::Paned(Gtk::Orientation::HORIZONTAL);
        paned.set_start_child(*navigationPanel_);
        paned.set_end_child(*contentView_);
        paned.set_position(280); // Largura inicial do painel de navegação
        paned.set_resize_start_child(true);
        paned.set_shrink_start_child(false);

        // 4. Cria uma caixa vertical para organizar o conteúdo principal e a barra de status
        Gtk::Box* tab_content_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
        tab_content_box->append(paned); // Adiciona o divisor no topo (ocupando a maior parte do espaço)
        tab_content_box->append(*statusBar_); // Adiciona a barra de status no rodapé

        // 5. Adiciona o layout completo a uma nova aba no AdwTabView
        adw_tab_view_append(tabView_, GTK_WIDGET(tab_content_box->gobj()));

        // 6. Define o AdwTabView como o conteúdo principal da janela
        // Usamos Gtk::manage para que o gtkmm gerencie a memória do widget C
        set_child(*Glib::wrap(GTK_WIDGET(tabView_)));
    }
}