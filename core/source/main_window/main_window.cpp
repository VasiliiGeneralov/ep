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

  connect(this, &MainWindow::openFileTriggered, this,
          &MainWindow::createNewContentTab);
  connect(tabManager, &QTabWidget::tabCloseRequested, this,
          &MainWindow::closeFile);
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
  emit openFileTriggered(fileName);
}

// TODO(v.generalov): lame solution, design custom dialog to accept both files
// and directories
void MainWindow::openDir() {
  QString fileName =
      QFileDialog::getExistingDirectory(this, "Open a directory");
  emit openFileTriggered(fileName);
}

void MainWindow::createNewContentTab(const QString &fileName, int index) {
  TabContentBuilder *builder = nullptr;

  QFileInfo info(fileName);
  if (info.isDir()) {
    builder = new FileTreeContentBuilder(this, fileName);
    connect(builder, SIGNAL(viewTriggered(QString, QWidget *)), this,
            SLOT(processViewTrigger(QString, QWidget *)));
  } else if (info.isFile()) {
    builder = new PlainTextContenBuilder(this, fileName);
  }

  if (!builder) {
    return;
  }

  connect(builder, &TabContentBuilder::builderReady, this,
          &MainWindow::populateContentTab);
  emit builder->builderReady(builder, index);
}

void MainWindow::populateContentTab(TabContentBuilder *builder, int index) {
  QWidget *tab = builder->populateTabContent();
  if (!tab) {
    return;
  }

  QString title = builder->getTitle();
  widgetToFilename.insert(tab, title);

  if (-1 == index) {
    int index = tabManager->addTab(tab, title);
    tabManager->setCurrentIndex(index);
    return;
  }

  tabManager->insertTab(index, tab, title);
  tabManager->setCurrentIndex(index);
}

bool MainWindow::saveFile(QWidget *widget) {
  // TODO(v.generalov): quit this dynamic_cast nonsense
  auto *tab = dynamic_cast<QPlainTextEdit *>(widget);
  if (!tab) {
    return true;
  }

  QString fileName = widgetToFilename.take(tab);
  QFile file(fileName);

  if (!file.open(QFile::Text | QFile::WriteOnly)) {
    QMessageBox::warning(this, "Warning",
                         "Cannot open file: " + file.errorString());
    return false;
  }

  file.resize(0);
  QTextStream oTextStream(&file);
  oTextStream << tab->toPlainText();

  return true;
}

void MainWindow::closeFile(int index) {
  auto *tab = tabManager->widget(index);
  if (!tab) {
    return;
  }

  int ret = QMessageBox::Discard;
  if (tab->isWindowModified()) {
    QMessageBox mBox(
        QMessageBox::Icon::Warning, "File is modified", "Keep changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, this);

    ret = mBox.exec();
  }

  bool status = true;
  switch (ret) {
  case QMessageBox::Save:
    status = saveFile(tab);
    break;
  case QMessageBox::Discard:
    // no action required before closing
    break;
  case QMessageBox::Cancel:
    [[fallthrough]];
  default:
    // either canceled or unknown return code, do nothing
    return;
  }

  if (!status) {
    return;
  }

  tabManager->removeTab(index);
}

void MainWindow::processViewTrigger(QString fileNameToOpen, QWidget *view) {
  int index = tabManager->indexOf(view);
  tabManager->removeTab(index);
  createNewContentTab(fileNameToOpen, index);
}
