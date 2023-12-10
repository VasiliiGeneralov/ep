#pragma once

#include "tab_content/tab_content.hpp"
#include <QAction>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QObject>
#include <QString>
#include <QTabWidget>
#include <QWidget>

namespace ep {

class MainWindow : public QMainWindow {
  // could be QPointer<QTabWidget>, yet it shouldn't be exposed outside of main
  // window, therefore main window's and tab manager's lifetimes won't differ in
  // any case
  QTabWidget *tabManager = nullptr;

  // Q_OBJECT macro in private section is mandatory for any QObject* inheriting
  // class, signals/slots may exhibit weird behavior otherwise
  Q_OBJECT

  class MenuActionFactory {
  public:
    // specifying a parent because QMenu::addAction() doesn't reparent a QAction
    // pointer
    virtual QList<QAction *> createActions(QMainWindow *parent) const = 0;
  };

  class FileMenuActFact : public MenuActionFactory {
  public:
    QList<QAction *> createActions(QMainWindow *parent) const override;
  };

  class HelpMenuActFact : public MenuActionFactory {
    QList<QAction *> createActions(QMainWindow *parent) const override;
  };

  void setMenuActions(QMenu *menu, const MenuActionFactory &fact);

  void setupMenuBar();
  void setupStatusBar();

  void setupTabManager();

  bool saveFile(TabContent *tab);

public:
  ~MainWindow() override = default;

  MainWindow() : QMainWindow() {
    setupMenuBar();
    setupStatusBar();

    setupTabManager();
  }

public slots:
  void createNewContentTab(const QString &fileName);
  void populateContentTab(TabContent *tab);
  void closeFile(int index);
  void openFile();
  void openGithub();
  void closeAllTabs();

signals:
  void openFileTriggered(const QString &fileName);
};

} // namespace ep
