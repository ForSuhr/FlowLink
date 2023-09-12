#ifndef PROGRESS_WINDOW_H
#define PROGRESS_WINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QProgressBar>

class ProgressWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = nullptr);

    void setupUI();
    QVBoxLayout *layout();
    void createProgressWidget();

private:
    QVBoxLayout *m_vbox = nullptr;
};

#endif // PROGRESS_WINDOW_H