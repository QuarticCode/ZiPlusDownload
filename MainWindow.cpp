#include "MainWindow.h"

#include <QWidget>
#include <QHBoxLayout>

#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#include <QProgressBar>
#include <QTextEdit>

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("ZiPlus Download");
    resize(700, 500);

    //----------------------------------
    // Crear proceso yt-dlp
    //----------------------------------

    process = new QProcess(this);

    //----------------------------------
    // Capturar salida estándar
    //----------------------------------

    connect(
        process,
        &QProcess::readyReadStandardOutput,
        this,
        [this]()
        {
            QString output =
                process->readAllStandardOutput();

            logArea->append(output);

            QRegularExpression re(R"((\d+\.\d+)%)");

            auto match = re.match(output);

            if(match.hasMatch())
            {
                double value =
                    match.captured(1).toDouble();

                progressBar->setValue(
                    static_cast<int>(value)
                );
            }
        }
    );

    //----------------------------------
    // Capturar errores
    //----------------------------------

    connect(
        process,
        &QProcess::readyReadStandardError,
        this,
        [this]()
        {
            QString error =
                process->readAllStandardError();

            logArea->append(error);
        }
    );

    //----------------------------------
    // Construir UI
    //----------------------------------

    QWidget* centralWidget =
        new QWidget(this);

    auto* mainLayout =
        new QVBoxLayout(centralWidget);

    //----------------------------------
    // URL
    //----------------------------------

    urlInput = new QLineEdit();

    urlInput->setPlaceholderText(
        "Pega una URL de YouTube"
    );

    mainLayout->addWidget(urlInput);

    //----------------------------------
    // Calidad
    //----------------------------------

    qualityCombo = new QComboBox();

    qualityCombo->addItem("1080p");
    qualityCombo->addItem("720p");
    qualityCombo->addItem("480p");
    qualityCombo->addItem("MP3");

    mainLayout->addWidget(qualityCombo);

    //----------------------------------
    // Carpeta
    //----------------------------------

    auto* folderLayout =
        new QHBoxLayout();

    folderInput = new QLineEdit();

    browseButton =
        new QPushButton("Examinar");

    folderLayout->addWidget(folderInput);
    folderLayout->addWidget(browseButton);

    mainLayout->addLayout(folderLayout);

    //----------------------------------
    // Botón descargar
    //----------------------------------

    downloadButton =
        new QPushButton("Descargar");

    mainLayout->addWidget(downloadButton);

    //----------------------------------
    // Barra progreso
    //----------------------------------

    progressBar =
        new QProgressBar();

    progressBar->setRange(0,100);
    progressBar->setValue(0);

    mainLayout->addWidget(progressBar);

    //----------------------------------
    // Logs
    //----------------------------------

    logArea =
        new QTextEdit();

    logArea->setReadOnly(true);

    mainLayout->addWidget(logArea);

    setCentralWidget(centralWidget);

    //----------------------------------
    // Eventos UI
    //----------------------------------

    connect(
        browseButton,
        &QPushButton::clicked,
        this,
        &MainWindow::browseFolder
    );

    connect(
        downloadButton,
        &QPushButton::clicked,
        this,
        &MainWindow::startDownload
    );

    QSettings settings("ZiPlus", "ZiPlusDownload");

    folderInput->setText(
        settings.value(
            "download_folder",
            ""
        ).toString()
    );
}

void MainWindow::browseFolder()
{
    QString folder =
        QFileDialog::getExistingDirectory(
            this,
            "Seleccionar carpeta"
        );

    if(!folder.isEmpty())
    {
        folderInput->setText(folder);

        QSettings settings(
            "ZiPlus",
            "ZiPlusDownload"
        );

        settings.setValue(
            "download_folder",
            folder
        );
    }
}

void MainWindow::startDownload() {
    QString url =
        urlInput->text();

    if(url.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Error",
            "Debes introducir una URL"
        );

        return;
    }

    progressBar->setValue(0);

    logArea->clear();

    //----------------------------------
    // IMPORTANTE:
    // Cambia esta ruta según tu Mac
    //----------------------------------

    QString ytDlpPath =
        "/opt/homebrew/bin/yt-dlp";

    QStringList arguments;

    //----------------------------------
    // PRUEBA INICIAL
    //----------------------------------

    QString quality =
    qualityCombo->currentText();

    if (quality == "MP3")
    {
        arguments << "-x";
        arguments << "--audio-format";
        arguments << "mp3";
        arguments << "--audio-quality";
        arguments << "0";
        arguments << "--embed-thumbnail";
        arguments << "--add-metadata";
    } else {
        if (quality == "1080p")
        {
            arguments << "-f"
                      << "bestvideo[height<=1080]+bestaudio/best[height<=1080]";
        }
        else if (quality == "720p")
        {
            arguments << "-f"
                      << "bestvideo[height<=720]+bestaudio/best[height<=720]";
        }
        else if (quality == "480p")
        {
            arguments << "-f"
                      << "bestvideo[height<=480]+bestaudio/best[height<=480]";
        }
        arguments << "--merge-output-format";
        arguments << "mp4";
    }


    arguments << "-P";
    arguments << folderInput->text();

    arguments << "--newline";

    arguments << url;

    logArea->append(
        "Ejecutando yt-dlp..."
    );

    process->start(
        ytDlpPath,
        arguments
    );
}