#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QDir>
#include <QFileInfo>
#include "cipher.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSelectInputFile();
    void onSelectKeyFile();
    void onSelectOutputFile();
    void onSelectDecryptInputFile();
    void onSelectDecryptOutputFile();
    void onEncryptClicked();
    void onDecryptClicked();
    void onGenerateKeyClicked();
    void updateLog(const QString &message);

private:
    void setupUI();
    void setupConnections();
    bool validateEncryptInputs();
    bool validateDecryptInputs();

    QLineEdit *encryptInputFileEdit;
    QLineEdit *keyFileEdit;
    QLineEdit *outputBinEdit;
    QLineEdit *outputTxtEdit;
    QPushButton *encryptButton;
    QGroupBox *encryptionGroup;

    QLineEdit *decryptInputFileEdit;
    QLineEdit *decryptKeyFileEdit;
    QLineEdit *decryptOutputEdit;
    QPushButton *decryptButton;
    QGroupBox *decryptionGroup;

    QPushButton *generateKeyButton;
    QGroupBox *keyManagementGroup;

    QTextEdit *logTextEdit;
    QProgressBar *progressBar;
};

#endif