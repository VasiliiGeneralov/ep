#include "tab_content/tab_content_wrapper.hpp"
#include "main_window/main_window.hpp"
#include "tab_content/tab_builder.hpp"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <cassert>

using namespace ep;

TabContentBuilder *PlainTextContentWrapper::createBuilder(MainWindow *parent) {
  if (!builder) {
    assert(!getFilename().isEmpty() && "Text builder got empty strin!");

    builder = new PlainTextContentBuilder(parent, getFilename());
    connect(builder, &PlainTextContentBuilder::builderReady, parent,
            &MainWindow::populateContentTab);
    connect(builder, &PlainTextContentBuilder::widgetCreated, this,
            &PlainTextContentWrapper::setWidget);
  }

  return builder;
}

FileHandler *PlainTextContentWrapper::createHandler() { return &handler; }

bool PlainTextFileHandler::open(QWidget *parent) {
  assert(getOwner() && "Text owner is null!");

  QString fileName = QFileDialog::getOpenFileName(parent, "Open the file");
  if (fileName.isEmpty()) {
    return false;
  }

  getOwner()->setFilename(fileName);
  return true;
}

bool PlainTextFileHandler::save(QWidget *parent) {
  assert(getOwner()->getWidget() && "Saving null widget!");

  QFile file(getOwner()->getFilename());

  if (!file.open(QFile::Text | QFile::WriteOnly)) {
    QMessageBox::warning(parent, "Warning",
                         "Cannot open file: " + file.errorString());
    return false;
  }

  file.resize(0);
  QTextStream oTextStream(&file);
  oTextStream << getOwner()->getWidget()->toPlainText();

  return true;
}

bool PlainTextFileHandler::close(QWidget *parent) {
  assert(getOwner()->getWidget() && "Closing null widget!");

  int ret = QMessageBox::Discard;
  if (getOwner()->getWidget()->isWindowModified()) {
    QMessageBox mBox(
        QMessageBox::Icon::Warning, "File is modified", "Keep changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, parent);

    ret = mBox.exec();
  }

  bool status = true;
  switch (ret) {
  case QMessageBox::Save:
    status = save(parent);
    break;
  case QMessageBox::Discard:
    // no action required before closing
    break;
  case QMessageBox::Cancel:
    [[fallthrough]];
  default:
    // either canceled or unknown return code, do nothing
    return false;
  }

  return status;
}

TabContentBuilder *
FileTreeViewContentWrapper::createBuilder(MainWindow *parent) {
  if (!builder) {
    assert(!getFilename().isEmpty() && "Text builder got empty strin!");

    builder = new FileTreeContentBuilder(parent, getFilename());
    connect(builder, &FileTreeContentBuilder::builderReady, parent,
            &MainWindow::populateContentTab);
    connect(builder, &FileTreeContentBuilder::widgetCreated, this,
            &FileTreeViewContentWrapper::setWidget);
    connect(builder, &FileTreeContentBuilder::viewTriggered, parent,
            &MainWindow::processViewTrigger);
  }

  return builder;
}

FileHandler *FileTreeViewContentWrapper::createHandler() { return &handler; }

bool FileTreeViewHandler::open(QWidget *parent) {
  assert(getOwner() && "File view owner is null!");

  QString fileName =
      QFileDialog::getExistingDirectory(parent, "Open a directory");
  if (fileName.isEmpty()) {
    return false;
  }

  getOwner()->setFilename(fileName);
  return true;
}
