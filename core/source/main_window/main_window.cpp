#include "main_window/main_window.hpp"
#include "tab_content/tab_content.hpp"
#include <QAction>
#include <QDesktopServices>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMenuBar>
#include <QMessageBox>
#include <QObject>
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

  auto *openAct = new QAction("Open", parent);
  QObject::connect(openAct, SIGNAL(triggered()), parent, SLOT(openFile()));
  actions.push_back(openAct);

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

void MainWindow::createNewContentTab(const QString &fileName) {
  // not specifying a parent, QTabWidget()::addTab() reparents a QWidget pointer
  auto *tab = new TabContent(nullptr, fileName);
  connect(tab, &TabContent::contentTabCreated, this,
          &MainWindow::populateContentTab);
  connect(tab, &QPlainTextEdit::modificationChanged,
          [tab = tab](bool changed) { tab->setWindowModified(changed); });
  tabManager->addTab(tab, fileName);

  emit tab->contentTabCreated(tab);
}

void MainWindow::populateContentTab(TabContent *tab) {
  // TODO(v.generalov): might be a good place to use the builder pattern
  QString fileName = tab->getFilename();

  if (fileName.isEmpty()) {
    return;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, "Warning",
                         "Cannot open file: " + file.errorString());
    return;
  }

  QTextStream iTextStream(&file);
  QString text = iTextStream.readAll();
  tab->setPlainText(text);

  // QFile and QTextStream are RAII objects, destructors will close and flush
  // descriptor and stream automatically, hence no direct QFile::close(),
  // QTestStram::flush() calls are necessary
}

bool MainWindow::saveFile(TabContent *tab) {
  QFile file(tab->getFilename());

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
  // TODO(v.generalov): might be a good place to use the builder pattern
  auto *tab = dynamic_cast<TabContent *>(tabManager->widget(index));
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
