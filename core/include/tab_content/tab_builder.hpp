#pragma once

#include <QFile>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QTabWidget>
#include <QTreeView>
#include <QWidget>

namespace ep {

class TabContentBuilder : public QObject {
  Q_OBJECT

  QString fileName;

public:
  explicit TabContentBuilder(QObject *parent, const QString &fileName)
      : QObject(parent), fileName(fileName) {}
  virtual ~TabContentBuilder() = default;

  const QString &getFilename() const { return fileName; }

  virtual QString getTitle() const { return getFilename(); }
  virtual QWidget *populateTabContent() { return nullptr; }

signals:
  void builderReady(TabContentBuilder *builder, int index);
};

class PlainTextContenBuilder : public TabContentBuilder {
  Q_OBJECT

public:
  explicit PlainTextContenBuilder(QObject *parent, const QString &fileName)
      : TabContentBuilder(parent, fileName) {}
  ~PlainTextContenBuilder() override = default;

  QWidget *populateTabContent() override;
};

class FileTreeContentBuilder : public TabContentBuilder {
  Q_OBJECT

  QFileSystemModel *model = nullptr;
  QTreeView *view = nullptr;

public:
  explicit FileTreeContentBuilder(QObject *parent, const QString &fileName)
      : TabContentBuilder(parent, fileName) {}
  ~FileTreeContentBuilder() override = default;

  QString getTitle() const override { return {""}; }
  QWidget *populateTabContent() override;

public slots:
  void checkIfFileTriggered(const QModelIndex &index);

signals:
  void viewTriggered(QString fileNameToOpen, QWidget *view);
};

} // namespace ep
