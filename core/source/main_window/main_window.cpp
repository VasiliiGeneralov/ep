#include "main_window/main_window.hpp"
#include "tab_content/tab_builder.hpp"
#include <QAction>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMenuBar>
#include <QMessageBox>
#include <QObject>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QStatusBar>
#include <QTabWidget>
#include <QTextEdit>
#include <QUrl>
#include <QWidget>
#include <algorithm>
#include <cassert>

using namespace ep;

QList<QAction *>
MainWindow::FileMenuActFact::createActions(QMainWindow *parent) const {
  QList<QAction *> actions;

  auto *openFileAct = new QAction("Open File", parent);
  QObject::connect(openFileAct, SIGNAL(triggered()), parent, SLOT(openFile()));
  actions.push_back(openFileAct);

  auto *openDirAct = new QAction("Open Dir", parent);
  QObject::connect(openDirAct, SIGNAL(triggered()), parent, SLOT(openDir()));
  actions.push_back(openDirAct);

  auto *closeAct = new QAction("Close All", parent);
  QObject::connect(closeAct, SIGNAL(triggered()), parent, SLOT(closeAllTabs()));
  actions.push_back(closeAct);

  return actions;
}

void MainWindow::closeAllTabs() {
  QList<QPointer<QWidget>> tabs;
  int tabCount = tabManager->count();
  for (int idx = 0; idx < tabCount; ++idx) {
    tabs.emplace_back(tabManager->widget(idx));
  }

  for (const auto &widget : std::as_const(tabs)) {
    if (!widget) {
      continue;
    }

    int idx = tabManager->indexOf(widget);
    tabManager->setCurrentIndex(idx);

    closeFile(idx);
  }
}

QList<QAction *>
MainWindow::HelpMenuActFact::createActions(QMainWindow *parent) const {
  QList<QAction *> actions;

  auto *visitAct = new QAction("Visit Github", parent);
  QObject::connect(visitAct, SIGNAL(triggered()), parent, SLOT(openGithub()));
  actions.push_back(visitAct);

  return actions;
}

void MainWindow::openGithub() {
  QDesktopServices::openUrl(QUrl("https://github.com/VasiliiGeneralov/ep"));
}

void MainWindow::setMenuActions(QMenu *menu, const MenuActionFactory &fact) {
  // Qt containers imply creating a copy before iterating
  QList<QAction *> actions = fact.createActions(this);

  menu->addActions(actions);
}

void MainWindow::setupMenuBar() {
  QMenuBar *bar = menuBar();

  QMenu *fileMenu = bar->addMenu("File");
  // ok to call with a temporary created object because of const&
  setMenuActions(fileMenu, FileMenuActFact());

  QMenu *helpMenu = bar->addMenu("Help");
  setMenuActions(helpMenu, HelpMenuActFact());
}

void MainWindow::setupStatusBar() { statusBar(); }

void MainWindow::setupTabManager() {
  // not specifying a parent because QMainWindow::setCentralWidget() reparents
  // the QWidgetPointer
  tabManager = new QTabWidget();

  tabManager->setTabsClosable(true);

  setCentralWidget(tabManager);

  connect(this, &MainWindow::openTriggered, this,
          &MainWindow::createNewContentTab);
  connect(tabManager, &QTabWidget::tabCloseRequested, this,
          &MainWindow::closeFile);
}

void MainWindow::openFile() {
  auto *wrapper = new PlainTextContentWrapper(this);
  contentWrappers.push_back(wrapper);

  bool res = wrapper->createHandler()->open(this);
  if (!res) {
    return;
  }

  emit openTriggered(wrapper);
}

// TODO(v.generalov): lame solution, design custom dialog to accept both files
// and directories
void MainWindow::openDir() {
  // TODO(v.generalov): these two are clearly repeated, come up with something
  // to avoid error prone repetition
  auto *wrapper = new FileTreeViewContentWrapper(this);
  contentWrappers.push_back(wrapper);

  bool res = wrapper->createHandler()->open(this);
  if (!res) {
    return;
  }

  emit openTriggered(wrapper);
}

void MainWindow::createNewContentTab(TabContentWrapper *wrapper, int index) {
  TabContentBuilder *builder = wrapper->createBuilder(this);

  if (!builder) {
    return;
  }

  emit builder->builderReady(builder, index);
}

void MainWindow::populateContentTab(TabContentBuilder *builder, int index) {
  QWidget *tab = builder->populateTabContent();
  if (!tab) {
    return;
  }

  QString title = builder->getTitle();

  if (-1 == index) {
    index = tabManager->addTab(tab, title);
  } else {
    tabManager->insertTab(index, tab, title);
  }

  tabManager->setCurrentIndex(index);
}

TabContentWrapper *MainWindow::getWrapperByWidget(QWidget *widget) {
  auto it = std::find_if(std::begin(contentWrappers), std::end(contentWrappers),
                         [widget = widget](TabContentWrapper *wrapper) {
                           return wrapper->getQWidget() == widget;
                         });

  assert(it != std::end(contentWrappers) && "Failed to determine wrapper!");

  return *it;
}

void MainWindow::closeTab(int index) {
  TabContentWrapper *wrapper = getWrapperByWidget(tabManager->widget(index));
  auto it = std::find(std::begin(contentWrappers), std::end(contentWrappers),
                      wrapper);

  assert(it != std::end(contentWrappers) && "Bad tab close!");
  delete *it;

  contentWrappers.erase(it);
  tabManager->removeTab(index);
}

void MainWindow::closeFile(int index) {
  QWidget *tab = tabManager->widget(index);
  if (!tab) {
    return;
  }

  TabContentWrapper *wrapper = getWrapperByWidget(tab);
  bool res = wrapper->createHandler()->close(this);
  if (!res) {
    return;
  }

  closeTab(index);
}

void MainWindow::processViewTrigger(QString fileNameToOpen, QWidget *view) {
  int index = tabManager->indexOf(view);
  closeTab(index);
  // TODO(v.generalov): it's shortsighted to treat all opened files as plain
  // text
  auto *wrapper = new PlainTextContentWrapper(this);
  contentWrappers.push_back(wrapper);

  wrapper->setFilename(fileNameToOpen);
  emit openTriggered(wrapper, index);
}
