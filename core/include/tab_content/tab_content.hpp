#pragma once

#include <QObject>
#include <QPlainTextEdit>
#include <QString>
#include <QWidget>

namespace ep {

class TabContent : public QPlainTextEdit {
  Q_OBJECT

  QString fileName;

public:
  ~TabContent() override = default;

  TabContent(QWidget *parent, const QString &fileName)
      : QPlainTextEdit(parent), fileName(fileName) {}

  QString getFilename() const { return fileName; }

signals:
  // common Qt idiom to inherit and implement necessary signals and slots, yet
  // official Qt guidelines state, that signals/slots shouldn't use highly
  // specific interfaces to avoid tight class coupling
  void contentTabCreated(TabContent *tab);
};

} // namespace ep
