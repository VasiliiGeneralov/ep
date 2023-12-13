#include "tab_content/tab_builder.hpp"
#include <QFileSystemModel>
#include <QMessageBox>
#include <QModelIndex>
#include <QPlainTextEdit>
#include <QTreeView>
#include <QWidget>

using namespace ep;

QWidget *FileTreeContentBuilder::populateTabContent() {
  // not specifying a parent, QTabWidget()::addTab() reparents a QWidget pointer
  view = new QTreeView();

  model = new QFileSystemModel(view);
  view->setModel(model);

  QModelIndex index = model->setRootPath(getFilename());
  view->setRootIndex(index);

  connect(view, &QTreeView::doubleClicked, this,
          &FileTreeContentBuilder::checkIfFileTriggered);

  emit widgetCreated(view);

  return view;
}

void FileTreeContentBuilder::checkIfFileTriggered(const QModelIndex &index) {
  QFileInfo info = model->fileInfo(index);
  if (!info.isFile()) {
    return;
  }

  emit viewTriggered(info.absoluteFilePath(), view);
}

QWidget *PlainTextContentBuilder::populateTabContent() {
  QFile file(getFilename());

  if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
    // TODO(v.generalov): may crash, construct QMessageBox instance manually
    QMessageBox::warning(nullptr, "Warning",
                         "Cannot open file: " + file.errorString());
    return nullptr;
  }

  QTextStream iTextStream(&file);
  QString text = iTextStream.readAll();

  auto *tab = new QPlainTextEdit();
  tab->setPlainText(text);

  connect(tab, &QPlainTextEdit::modificationChanged,
          [tab = tab](bool changed) { tab->setWindowModified(changed); });

  emit widgetCreated(tab);

  return tab;

  // QFile and QTextStream are RAII objects, destructors will close and flush
  // descriptor and stream automatically, hence no direct QFile::close(),
  // QTestStram::flush() calls are necessary
}
