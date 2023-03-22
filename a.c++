#include <QMainWindow>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>


class Nodepad : public QMainWindow
{
    Q_OBJECT
public:
    Nodepad(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        // Create the text editor widget
        editor = new QTextEdit(this);
        setCentralWidget(editor);

        // Create the File menu
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

        // Create the New action
        QAction *newAction = new QAction(tr("&New"), this);
        newAction->setShortcut(QKeySequence::New);
        fileMenu->addAction(newAction);

        // Connect the New action to the createNewFile() slot
        connect(newAction, &QAction::triggered, this, &Nodepad::createNewFile);

        // Create the Open action
        QAction *openAction = new QAction(tr("&Open..."), this);
        openAction->setShortcut(QKeySequence::Open);
        fileMenu->addAction(openAction);

        // Connect the Open action to the openFile() slot
        connect(openAction, &QAction::triggered, this, &Nodepad::openFile);

        // Create the Save action
        QAction *saveAction = new QAction(tr("&Save"), this);
        saveAction->setShortcut(QKeySequence::Save);
        fileMenu->addAction(saveAction);

        // Connect the Save action to the saveFile() slot
        connect(saveAction, &QAction::triggered, this, &Nodepad::saveFile);

        // Create the Save As action
        QAction *saveAsAction = new QAction(tr("Save &As..."), this);
        fileMenu->addAction(saveAsAction);

        // Connect the Save As action to the saveAsFile() slot
        connect(saveAsAction, &QAction::triggered, this, &Nodepad::saveAsFile);

        // Create the Exit action
        QAction *exitAction = new QAction(tr("&Exit"), this);
        exitAction->setShortcut(QKeySequence::Quit);
        fileMenu->addAction(exitAction);

        // Connect the Exit action to the close() slot
        connect(exitAction, &QAction::triggered, this, &Nodepad::close);

        // Set the window properties
        setWindowTitle(tr("Nodepad"));
        setMinimumSize(640, 480);
    }

private slots:
    // Slot for creating a new file
    void createNewFile()
    {
        // Clear the editor
        editor->clear();

        // Set the file name to "Untitled"
        fileName = "Untitled";

        // Set the saved flag to false
        isSaved = false;

        // Show a message box
        QMessageBox::information(this, tr("Nodepad"), tr("New file created."));
    }

    // Slot for opening a file
    void openFile()
    {
        // Get the file name from a file dialog
        QString newFileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Text Files (*.txt);;All Files (*)"));

        // Check if the user canceled the dialog
        if (newFileName.isNull())
        {
            return;
            // Open the file and read its contents
            QFile file(newFileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("Nodepad"), tr("Failed to open file."));
                return;
            }
            QTextStream in(&file);
            editor->setPlainText(in.readAll());
            file.close();
            // Set the file name and saved flag
            fileName = newFileName;
            isSaved = true;

            // Show a message box
            QMessageBox::information(this, tr("Nodepad"), tr("File opened."));
        }

        // Slot for saving the current file
        void saveFile()
        {
            // If the file has not been saved yet, call saveAsFile() instead
            if (!isSaved)
            {
                saveAsFile();
                return;
            }

            // Open the file and write the contents
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("Nodepad"), tr("Failed to save file."));
                return;
            }
            QTextStream out(&file);
            out << editor->toPlainText();
            file.close();

            // Set the saved flag
            isSaved = true;

            // Show a message box
            QMessageBox::information(this, tr("Nodepad"), tr("File saved."));
        }

        // Slot for saving the current file as a new file
        void saveAsFile()
        {
            // Get the file name from a file dialog
            QString newFileName = QFileDialog::getSaveFileName(this, tr("Save File As"), QString(), tr("Text Files (*.txt);;All Files (*)"));

            // Check if the user canceled the dialog
            if (newFileName.isNull())
            {
                return;
            }

            // Open the file and write the contents
            QFile file(newFileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("Nodepad"), tr("Failed to save file."));
                return;
            }
            QTextStream out(&file);
            out << editor->toPlainText();
            file.close();

            // Set the file name and saved flag
            fileName = newFileName;
            isSaved = true;

            // Show a message box
            QMessageBox::information(this, tr("Nodepad"), tr("File saved."));
        }

    private:
        QTextEdit *editor;
        QString fileName;
        bool isSaved;
    };

    int main(int argc, char *argv[])
    {
        QApplication app(argc, argv);
        Nodepad nodepad;
        nodepad.show();
        return app.exec();
    }