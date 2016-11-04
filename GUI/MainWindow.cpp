#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "SmartScene.hpp"

//Size of the scene
const uint MainWindow::Width = 800;
const uint MainWindow::Height = 600;

//Define color variables
const uint MainWindow::RED = 1;
const uint MainWindow::YELLOW = 2;

//Default number of GUIs
uint MainWindow::GUICount = 0;

//Use for comparing QString*s
struct compareQString {
    bool operator () (const QString* a, const QString* b)
    {return *a < *b; }
};

//Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), StrokeThickness(4) {

    //Make sure only one GUI exists
    Assert(!MainWindow::GUICount, "only 1 GUI can exist");
    MainWindow::GUICount = 1;

    //Initalize variables
    theCourse = new QString("");

    //Setup the ui
    ui->setupUi(this);
    ui->centralWidget->setFixedSize(Width, Height);
    ui->graphicsView->setFixedSize(Width, Height);
    ui->graphicsView->setScene(new SmartScene());

    //Disable scrolling
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    //Disable autocomplete for course number box
    ui->CourseNumber->setCompleter(NULL);

	//Draw outlines
    drawOutlines();

    //Conect everything up
    connectDefaults();

    //Draw the GUI
    updateClassesTaken();
}

//Destructor
MainWindow::~MainWindow() {

    //UI destructor calls destructor of all items it has
    delete ui;

#if 0 //TODO: figure out if these must be deleted
    //Record information to delete
    auto theScene = ui->graphicsView->scene();
    //Must delete destructed items still
#endif
}

//Draw the outlines of each section
void MainWindow::drawOutlines() {

    //For organization
    int CoursesH = Width/5;
    int MainWidth = Width/2;
    int TopHeight = Height/8;

    //Create a specialized pen
    QPen p; p.setCosmetic(false);
    p.setWidth(StrokeThickness);

    //For readability
    const int ST = StrokeThickness/2;
    auto theScene = ui->graphicsView->scene();

    //Top major selection
    Outlines.push_back(theScene->addRect(ST, ST, MainWidth-ST, TopHeight-ST, p));

	//Main requirement
    Outlines.push_back(theScene->addRect(ST, TopHeight, MainWidth-ST, Height-TopHeight-ST, p));

	//Hass requirement
    Outlines.push_back(theScene->addRect(MainWidth, ST, MainWidth/2, Height-2*ST, p));

	//Enter courses
    Outlines.push_back(theScene->addRect((3*MainWidth)/2, ST, MainWidth/2-ST, CoursesH-ST, p));

	//Courses entered
    Outlines.push_back(theScene->addRect((3*MainWidth)/2, CoursesH, MainWidth/2-ST, Height-CoursesH-ST, p));
}

void MainWindow::connectDefaults() {

    //Connect the toggle switch to the toggle slot
    QObject::connect(ui->toggleTentaive, SIGNAL(toggled(bool)),
                     this, SLOT(tentativeToggle(bool)));

    //Connect add class button to the addClass function
    QObject::connect(ui->addClassButton, SIGNAL(pressed()),
                     this, SLOT(addClass()));

    //Connect remove class button to the removeClass function
    QObject::connect(ui->removeClassButton, SIGNAL(pressed()),
                     this, SLOT(removeClass()));

    //Set the defaults
    tentativeToggle(true);
}

//Set the positions of everything
//void PositionObjects(int,int); //TODO: Implement

//Called to enable or disable
//tentative class adding/removing
void MainWindow::tentativeToggle(bool checked) {

    //Enable auto update
    if (checked) {

       //Connect course Major box
       QObject::connect(ui->CourseMajor, SIGNAL(currentTextChanged(QString)),
                        this, SLOT(tentativelyAlterClasses(QString)));
       QObject::connect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                        this, SLOT(tentativelyAlterClasses(QString)));

       //Connect course Number box
       QObject::connect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                        this, SLOT(tentativelyAlterClasses(QString)));
       QObject::connect(ui->CourseNumber, SIGNAL(editTextChanged(QString)),
                        this, SLOT(tentativelyAlterClasses(QString)));
    }

    //Disable auto update
    else {

       //Disconnect course Major box
       QObject::disconnect(ui->CourseMajor, SIGNAL(currentTextChanged(QString)),
                        this, SLOT(tentativelyAddClass(QString)));

       QObject::disconnect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                        this, SLOT(tentativelyAddClass(QString)));

       //Disconnect course Number box
       QObject::disconnect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                        this, SLOT(tentativelyAddClass(QString)));
       QObject::disconnect(ui->CourseNumber, SIGNAL(editTextChanged(QString)),
                        this, SLOT(tentativelyAddClass(QString)));
    }
}

//Update the definition of theCourse
void MainWindow::updateCourse() {
    delete theCourse;
    theCourse = new QString(" ");
    theCourse->prepend(ui->CourseMajor->currentText());
    theCourse->append(ui->CourseNumber->currentText());
}

#include <QDebug>
//Called if the tentative class selection changed
void MainWindow::tentativelyAlterClasses(const QString&) {

    //Update theCourse
    updateCourse();

    //If the class is new, tentatively add it, update the GUI
    if (classesTaken.find(theCourse) == classesTaken.end())
        updateClassesTaken(YELLOW);

    //Otherwise, tentatively remove it, update the GUI
    else updateClassesTaken(RED);
}

//Add or remove a class
void MainWindow::removeClass() {

    //Update theCourse
    updateCourse();

    qDebug() << "Remove called!";
    //Remove the class if it exists
    auto tmp = classesTaken.find(theCourse);
    if (tmp != classesTaken.end()) {
        delete *tmp; classesTaken.erase(tmp);
    }

    //Update the GUI
    updateClassesTaken();
}

void MainWindow::addClass() {

    //Update theCourse
    updateCourse();

    qDebug() << "Add called!";
    classesTaken.insert(new QString(*theCourse));
    updateClassesTaken();
}

//Update the GUI's classes take list, and update the rest subsequently
void MainWindow::updateClassesTaken(uint Highlight) {
    qDebug() << "Update called!";
    //TODO: CALL OTHER FUNCTIONS HERE

}
