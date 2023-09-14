#ifndef PROGRESS_WINDOW_H
#define PROGRESS_WINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QProgressBar>
#include <QDebug>

#include <memory>

#include "WindowsGlobals.h"

class ProgressWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = nullptr);
    ~ProgressWindow();

    void setupUI();
    QVBoxLayout *layout();
    void createProgressWidget(const QString &filename, qint64 totalFileBytes);
    void updateProgress(const QString &filename, qint64 bytes, qint64 totalBytes);

public slots:
    void deleteProgressWidget();

private:
    QVBoxLayout *m_vbox = nullptr;
    std::unique_ptr<std::unordered_map<QString, QWidget *>> m_progressWidgetMap = nullptr; // a map used to store progress widgets, one widget per downloading file
};

#endif // PROGRESS_WINDOW_H