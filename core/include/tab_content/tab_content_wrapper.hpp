#pragma once

#include "tab_content/tab_builder.hpp"
#include <QObject>
#include <QPlainTextEdit>
#include <QString>
#include <QWidget>

namespace ep {

class TabContentWrapper;

class FileHandler : public QObject {
public:
  virtual ~FileHandler() = default;

  // specify parent widget to display dialog at proper position
  virtual bool open(QWidget *parent) { return true; }
  virtual bool close(QWidget *parent) { return true; }
  virtual bool save(QWidget *parent) { return true; }
};

class PlainTextContentWrapper;

class PlainTextFileHandler : public FileHandler {
  PlainTextContentWrapper *owner;

public:
  PlainTextFileHandler(PlainTextContentWrapper *owner) : owner(owner) {}
  ~PlainTextFileHandler() override = default;

  PlainTextContentWrapper *getOwner() { return owner; }

  bool open(QWidget *parent) override;
  bool close(QWidget *parent) override;
  bool save(QWidget *parent) override;
};

class FileTreeViewContentWrapper;

class FileTreeViewHandler : public FileHandler {
  FileTreeViewContentWrapper *owner = nullptr;

public:
  FileTreeViewHandler(FileTreeViewContentWrapper *owner) : owner(owner) {}
  ~FileTreeViewHandler() override = default;

  FileTreeViewContentWrapper *getOwner() { return owner; }

  bool open(QWidget *parent) override;
};

// forward declaration to avoid circular include dependency
class MainWindow;

class TabContentWrapper : public QObject {
  Q_OBJECT

  QString fileName;

public:
  TabContentWrapper(QObject *parent) : QObject(parent) {}
  virtual ~TabContentWrapper() = default;

  void setFilename(const QString &newFilename) { fileName = newFilename; }
  const QString &getFilename() { return fileName; }

  virtual QWidget *getQWidget() = 0;

  virtual TabContentBuilder *createBuilder(MainWindow *parent) = 0;
  virtual FileHandler *createHandler() = 0;
};

class PlainTextContentWrapper : public TabContentWrapper {
  Q_OBJECT

  PlainTextFileHandler handler;
  PlainTextContentBuilder *builder = nullptr;

  QPlainTextEdit *widget = nullptr;

public:
  PlainTextContentWrapper(QObject *parent)
      : TabContentWrapper(parent), handler(this) {}
  ~PlainTextContentWrapper() override = default;

  QWidget *getQWidget() override { return widget; }
  QPlainTextEdit *getWidget() { return widget; }

  TabContentBuilder *createBuilder(MainWindow *parent) override;
  FileHandler *createHandler() override;

public slots:
  void setWidget(QPlainTextEdit *newWidget) { widget = newWidget; }
};

class FileTreeViewContentWrapper : public TabContentWrapper {
  Q_OBJECT

  FileTreeContentBuilder *builder = nullptr;
  FileTreeViewHandler handler;

  QTreeView *widget = nullptr;

public:
  FileTreeViewContentWrapper(QObject *parent)
      : TabContentWrapper(parent), handler(this) {}
  ~FileTreeViewContentWrapper() override = default;

  QWidget *getQWidget() override { return widget; }
  TabContentBuilder *createBuilder(MainWindow *parent) override;
  FileHandler *createHandler() override;

public slots:
  void setWidget(QTreeView *newWidget) { widget = newWidget; }
};

} // namespace ep
