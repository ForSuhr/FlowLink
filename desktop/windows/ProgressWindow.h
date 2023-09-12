#ifndef PROGRESS_WINDOW_H
#define PROGRESS_WINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include "WindowsGlobals.h"

class ProgressWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = nullptr);

    void setupUI();
    QVBoxLayout *layout();
    void createProgressWidget(const QString &filename, qint64 fileTotalBytes);
    void updateProgress(int bytes);

private:
    QVBoxLayout *m_vbox = nullptr;
    QLabel *fileCurrentSizeLabel = nullptr;
    QProgressBar *progressBar = nullptr;
    std::unordered_map<QString, QWidget *> m_progressWidgetMap = {}; // a map used to store progress widgets, one widget per downloading file
};

#endif // PROGRESS_WINDOW_H