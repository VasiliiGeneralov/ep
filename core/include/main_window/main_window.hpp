#pragma once

#include "tab_content/tab_builder.hpp"
#include "tab_content/tab_content_wrapper.hpp"
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

  QList<TabContentWrapper *> contentWrappers;

  TabContentWrapper *getWrapperByWidget(QWidget *widget);
  void closeTab(int index);

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

public:
  ~MainWindow() override = default;

  MainWindow() : QMainWindow() {
    setupMenuBar();
    setupStatusBar();

    setupTabManager();
  }

public slots:
  // common Qt idiom to inherit and implement necessary signals and slots, yet
  // official Qt guidelines state, that signals/slots shouldn't use highly
  // specific interfaces to avoid tight class coupling
  void createNewContentTab(TabContentWrapper *wrapper, int index = -1);
  void populateContentTab(TabContentBuilder *builder, int index);
  void closeFile(int index);
  void openFile();
  void openDir();
  void openGithub();
  void closeAllTabs();
  void processViewTrigger(QString fileNameToOpen, QWidget *view);

signals:
  void openTriggered(TabContentWrapper *wrapper, int index = -1);
};

} // namespace ep
