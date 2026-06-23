#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

class QLineEdit;
class QPushButton;
class QComboBox;
class QProgressBar;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QLineEdit* urlInput;
    QLineEdit* folderInput;

    QPushButton* browseButton;
    QPushButton* downloadButton;

    QComboBox* qualityCombo;

    QProgressBar* progressBar;

    QTextEdit* logArea;

    QProcess* process;

private slots:
    void browseFolder();
    void startDownload();
};

#endif