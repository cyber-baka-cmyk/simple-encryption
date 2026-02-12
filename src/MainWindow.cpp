#include "MainWindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setupConnections();

    setWindowTitle("Cipher Application");
    setMinimumSize(700, 550);

    updateLog("Application ready. Generate or select a key file to begin.");
}

MainWindow::~MainWindow() { }

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);

    keyManagementGroup = new QGroupBox("Key Management", this);
    QHBoxLayout *keyLayout = new QHBoxLayout(keyManagementGroup);

    keyLayout->addWidget(new QLabel("Key File:", this));

    keyFileEdit = new QLineEdit(this);
    keyFileEdit->setReadOnly(true);
    keyFileEdit->setPlaceholderText("No key file selected");
    keyLayout->addWidget(keyFileEdit);

    QPushButton *browseKeyBtn = new QPushButton("Browse...", this);
    connect(browseKeyBtn, &QPushButton::clicked, this, &MainWindow::onSelectKeyFile);
    keyLayout->addWidget(browseKeyBtn);

    generateKeyButton = new QPushButton("Generate New Key", this);
    generateKeyButton->setStyleSheet("QPushButton { background-color: #FF9800; color: white; font-weight: bold; padding: 8px 15px; }");
    keyLayout->addWidget(generateKeyButton);

    mainLayout->addWidget(keyManagementGroup);

    encryptionGroup = new QGroupBox("Encryption", this);
    QGridLayout *encryptLayout = new QGridLayout(encryptionGroup);

    encryptLayout->addWidget(new QLabel("Source File:", this), 0, 0);
    encryptInputFileEdit = new QLineEdit(this);
    encryptInputFileEdit->setReadOnly(true);
    encryptInputFileEdit->setPlaceholderText("Select a file to encrypt");
    encryptLayout->addWidget(encryptInputFileEdit, 0, 1);
    QPushButton *browseEncryptInputBtn = new QPushButton("Browse...", this);
    connect(browseEncryptInputBtn, &QPushButton::clicked, this, &MainWindow::onSelectInputFile);
    encryptLayout->addWidget(browseEncryptInputBtn, 0, 2);

    encryptLayout->addWidget(new QLabel("Output (.bin):", this), 1, 0);
    outputBinEdit = new QLineEdit(this);
    outputBinEdit->setReadOnly(true);
    outputBinEdit->setPlaceholderText("Binary output file");
    encryptLayout->addWidget(outputBinEdit, 1, 1);
    QPushButton *browseBinBtn = new QPushButton("Browse...", this);
    connect(browseBinBtn, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getSaveFileName(this, "Save Binary Output", QDir::homePath(), "Binary files (*.bin)");
        if (!file.isEmpty()) {
            if (!file.endsWith(".bin", Qt::CaseInsensitive)) file += ".bin";
            outputBinEdit->setText(file);
            updateLog("Binary output set: " + file);
        }
    });
    encryptLayout->addWidget(browseBinBtn, 1, 2);

    encryptLayout->addWidget(new QLabel("Output (.txt):", this), 2, 0);
    outputTxtEdit = new QLineEdit(this);
    outputTxtEdit->setReadOnly(true);
    outputTxtEdit->setPlaceholderText("Hex output file");
    encryptLayout->addWidget(outputTxtEdit, 2, 1);
    QPushButton *browseTxtBtn = new QPushButton("Browse...", this);
    connect(browseTxtBtn, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getSaveFileName(this, "Save Hex Output", QDir::homePath(), "Text files (*.txt)");
        if (!file.isEmpty()) {
            if (!file.endsWith(".txt", Qt::CaseInsensitive)) file += ".txt";
            outputTxtEdit->setText(file);
            updateLog("Hex output set: " + file);
        }
    });
    encryptLayout->addWidget(browseTxtBtn, 2, 2);

    encryptButton = new QPushButton("ENCRYPT", this);
    encryptButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 12px; font-size: 14px; }");
    encryptLayout->addWidget(encryptButton, 3, 0, 1, 3);

    mainLayout->addWidget(encryptionGroup);

    decryptionGroup = new QGroupBox("Decryption", this);
    QGridLayout *decryptLayout = new QGridLayout(decryptionGroup);

    decryptLayout->addWidget(new QLabel("Input File (.bin):", this), 0, 0);
    decryptInputFileEdit = new QLineEdit(this);
    decryptInputFileEdit->setReadOnly(true);
    decryptInputFileEdit->setPlaceholderText("Select encrypted .bin file");
    decryptLayout->addWidget(decryptInputFileEdit, 0, 1);
    QPushButton *browseDecryptInputBtn = new QPushButton("Browse...", this);
    connect(browseDecryptInputBtn, &QPushButton::clicked, this, &MainWindow::onSelectDecryptInputFile);
    decryptLayout->addWidget(browseDecryptInputBtn, 0, 2);

    decryptLayout->addWidget(new QLabel("Key File:", this), 1, 0);
    decryptKeyFileEdit = new QLineEdit(this);
    decryptKeyFileEdit->setReadOnly(true);
    decryptKeyFileEdit->setPlaceholderText("Key file for decryption");
    decryptLayout->addWidget(decryptKeyFileEdit, 1, 1);
    QPushButton *browseDecryptKeyBtn = new QPushButton("Browse...", this);
    connect(browseDecryptKeyBtn, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(this, "Select Key File", QDir::homePath(), "Key files (*.key);;All files (*)");
        if (!file.isEmpty()) {
            decryptKeyFileEdit->setText(file);
            updateLog("Decryption key selected: " + file);
        }
    });
    decryptLayout->addWidget(browseDecryptKeyBtn, 1, 2);

    decryptLayout->addWidget(new QLabel("Output File:", this), 2, 0);
    decryptOutputEdit = new QLineEdit(this);
    decryptOutputEdit->setReadOnly(true);
    decryptOutputEdit->setPlaceholderText("Decrypted output file");
    decryptLayout->addWidget(decryptOutputEdit, 2, 1);
    QPushButton *browseDecryptOutputBtn = new QPushButton("Browse...", this);
    connect(browseDecryptOutputBtn, &QPushButton::clicked, this, &MainWindow::onSelectDecryptOutputFile);
    decryptLayout->addWidget(browseDecryptOutputBtn, 2, 2);

    decryptButton = new QPushButton("DECRYPT", this);
    decryptButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; padding: 12px; font-size: 14px; }");
    decryptLayout->addWidget(decryptButton, 3, 0, 1, 3);

    mainLayout->addWidget(decryptionGroup);

    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);
    progressBar->setTextVisible(false);
    mainLayout->addWidget(progressBar);

    mainLayout->addWidget(new QLabel("Activity Log:", this));
    logTextEdit = new QTextEdit(this);
    logTextEdit->setReadOnly(true);
    logTextEdit->setMaximumHeight(120);
    logTextEdit->setStyleSheet("QTextEdit { background-color: #f5f5f5; font-family: monospace; }");
    mainLayout->addWidget(logTextEdit);

    setCentralWidget(centralWidget);
    statusBar()->showMessage("Ready");
}

void MainWindow::setupConnections() {
    connect(encryptButton, &QPushButton::clicked, this, &MainWindow::onEncryptClicked);
    connect(decryptButton, &QPushButton::clicked, this, &MainWindow::onDecryptClicked);
    connect(generateKeyButton, &QPushButton::clicked, this, &MainWindow::onGenerateKeyClicked);
}

void MainWindow::updateLog(const QString &message) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    logTextEdit->append(QString("[%1] %2").arg(timestamp, message));
}

bool MainWindow::validateEncryptInputs() {
    if (encryptInputFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select a source file to encrypt.");
        return false;
    }
    if (keyFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select or generate a key file.");
        return false;
    }
    if (outputBinEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please specify a binary output file.");
        return false;
    }
    if (outputTxtEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please specify a hex output file.");
        return false;
    }
    return true;
}

bool MainWindow::validateDecryptInputs() {
    if (decryptInputFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select an encrypted .bin file to decrypt.");
        return false;
    }
    if (decryptKeyFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select a key file for decryption.");
        return false;
    }
    if (decryptOutputEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please specify an output file for the decrypted data.");
        return false;
    }
    return true;
}

void MainWindow::onSelectInputFile() {
    QString file = QFileDialog::getOpenFileName(this, "Select File to Encrypt", QDir::homePath(), "All files (*)");

    if (!file.isEmpty()) {
        encryptInputFileEdit->setText(file);
        updateLog("Selected source file: " + file);

        QFileInfo info(file);
        QString baseName = info.absolutePath() + "/" + info.completeBaseName();
        outputBinEdit->setText(baseName + "_encrypted.bin");
        outputTxtEdit->setText(baseName + "_hex.txt");
        updateLog("Auto-set output files");
    }
}

void MainWindow::onSelectKeyFile() {
    QString file = QFileDialog::getOpenFileName(this, "Select Key File", QDir::homePath(), "Key files (*.key);;All files (*)");

    if (!file.isEmpty()) {
        keyFileEdit->setText(file);
        updateLog("Selected key file: " + file);
    }
}

void MainWindow::onSelectOutputFile() { }

void MainWindow::onSelectDecryptInputFile() {
    QString file = QFileDialog::getOpenFileName(this, "Select Encrypted File", QDir::homePath(), "Binary files (*.bin);;All files (*)");

    if (!file.isEmpty()) {
        decryptInputFileEdit->setText(file);
        updateLog("Selected encrypted file: " + file);

        QFileInfo info(file);
        QString baseName = info.absolutePath() + "/" + info.completeBaseName();
        QString outputName = baseName;
        if (outputName.endsWith("_encrypted")) {
            outputName = outputName.left(outputName.length() - 10) + "_decrypted.txt";
        } else {
            outputName = baseName + "_decrypted.txt";
        }
        decryptOutputEdit->setText(outputName);
        updateLog("Auto-set output: " + outputName);
    }
}

void MainWindow::onSelectDecryptOutputFile() {
    QString file = QFileDialog::getSaveFileName(this, "Save Decrypted File", QDir::homePath(), "All files (*)");

    if (!file.isEmpty()) {
        decryptOutputEdit->setText(file);
        updateLog("Decryption output set: " + file);
    }
}

void MainWindow::onEncryptClicked() {
    if (!validateEncryptInputs()) return;

    progressBar->setVisible(true);
    progressBar->setRange(0, 0);
    encryptButton->setEnabled(false);
    decryptButton->setEnabled(false);

    updateLog("Starting encryption...");

    QString inputFile = encryptInputFileEdit->text();
    QString keyFile = keyFileEdit->text();
    QString outputBin = outputBinEdit->text();
    QString outputTxt = outputTxtEdit->text();

    QByteArray inputBa = inputFile.toUtf8();
    QByteArray keyBa = keyFile.toUtf8();
    QByteArray outputBinBa = outputBin.toUtf8();
    QByteArray outputTxtBa = outputTxt.toUtf8();

    encrypt_file(inputBa.constData(), keyBa.constData(), outputBinBa.constData(), outputTxtBa.constData());

    updateLog("Encryption completed!");
    updateLog("  Binary output: " + outputBin);
    updateLog("  Hex output: " + outputTxt);
    statusBar()->showMessage("Encryption completed", 5000);

    progressBar->setVisible(false);
    encryptButton->setEnabled(true);
    decryptButton->setEnabled(true);

    QMessageBox::information(this, "Success", "File encrypted successfully!\n\nBinary: " + outputBin + "\nHex: " + outputTxt);
}

void MainWindow::onDecryptClicked() {
    if (!validateDecryptInputs()) return;

    progressBar->setVisible(true);
    progressBar->setRange(0, 0);
    encryptButton->setEnabled(false);
    decryptButton->setEnabled(false);

    updateLog("Starting decryption...");

    QString inputFile = decryptInputFileEdit->text();
    QString keyFile = decryptKeyFileEdit->text();
    QString outputFile = decryptOutputEdit->text();

    QByteArray inputBa = inputFile.toUtf8();
    QByteArray keyBa = keyFile.toUtf8();
    QByteArray outputBa = outputFile.toUtf8();

    decrypt_file(inputBa.constData(), keyBa.constData(), outputBa.constData());

    updateLog("Decryption completed!");
    updateLog("  Output: " + outputFile);
    statusBar()->showMessage("Decryption completed", 5000);

    progressBar->setVisible(false);
    encryptButton->setEnabled(true);
    decryptButton->setEnabled(true);

    QMessageBox::information(this, "Success", "File decrypted successfully!\n\nOutput: " + outputFile);
}

void MainWindow::onGenerateKeyClicked() {
    QString defaultPath = QDir::homePath() + "/cipher_key.key";
    QString keyFile = QFileDialog::getSaveFileName(this, "Save Key File", defaultPath, "Key files (*.key);;All files (*)");

    if (!keyFile.isEmpty()) {
        if (!keyFile.endsWith(".key", Qt::CaseInsensitive)) {
            keyFile += ".key";
        }

        progressBar->setVisible(true);
        progressBar->setRange(0, 0);

        updateLog("Generating key: " + keyFile);

        QByteArray keyFileBa = keyFile.toUtf8();
        generate_key(keyFileBa.constData());

        keyFileEdit->setText(keyFile);
        decryptKeyFileEdit->setText(keyFile);

        updateLog("Key generated successfully!");
        statusBar()->showMessage("Key generated", 5000);

        progressBar->setVisible(false);

        QMessageBox::information(this, "Key Generated",
            "Key file created successfully!\n\nLocation: " + keyFile + "\nSize: 64 bytes (512 bits)");
    }
}