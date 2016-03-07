#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fl/Headers.h"
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include "dialog.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{


    using namespace fl;
fl::Engine* engine = new fl::Engine;
engine->setName("");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("temperatura");
inputVariable1->setRange(-20.000, 40.000);
inputVariable1->addTerm(new fl::Triangle("frio", -20.000, 0.000, 15.000));
inputVariable1->addTerm(new fl::Triangle("templado", 5.000, 16.000, 27.000));
inputVariable1->addTerm(new fl::Triangle("caliente", 23.000, 30.000, 40.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("humedad");
inputVariable2->setRange(0.000, 100.000);
inputVariable2->addTerm(new fl::Triangle("baja", 0.000, 20.000, 40.000));
inputVariable2->addTerm(new fl::Triangle("media", 35.000, 55.000, 75.000));
inputVariable2->addTerm(new fl::Triangle("alta", 70.000, 85.000, 100.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("velocidad");
outputVariable->setRange(0.000, 1000.000);

outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->setDefaultValue(fl::nan);

outputVariable->addTerm(new fl::Triangle("lenta", 0.000, 200.000, 400.000));
outputVariable->addTerm(new fl::Triangle("media", 300.000, 500.000, 700.000));
outputVariable->addTerm(new fl::Triangle("rapida", 600.000, 800.000, 1000.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if temperatura is frio and humedad is baja then velocidad is lenta", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is frio and humedad is media then velocidad is lenta", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is frio and humedad is alta then velocidad is lenta", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is templado and humedad is baja then velocidad is media", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is templado and humedad is media then velocidad is media", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is templado and humedad is alta then velocidad is lenta", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is caliente and humedad is baja then velocidad is rapida", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is caliente and humedad is media then velocidad is rapida", engine));
ruleBlock->addRule(fl::Rule::parse("if temperatura is caliente and humedad is alta then velocidad is media", engine));
engine->addRuleBlock(ruleBlock);


       engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid");
       std::string status;
       if (not engine->isReady(&status))
           throw Exception("Engine not ready. "
               "The following errors were encountered:\n" + status, FL_AT);
    QString humedad = ui->humedad->text();
       QString temperatura = ui->temperatura->text();

       inputVariable1->setInputValue(temperatura.toFloat());
       inputVariable2->setInputValue(humedad.toFloat());

       engine->process();
//       FL_LOG( "Power.output = " << Op::str(outputVariable->g ));


        float velocidad = outputVariable->getOutputValue();
      //  FL_LOG( "Power.output = " << velocidad);

        if( velocidad < 350 )
            ui->Print_Velocidad->setText(" Baja");

        else if(velocidad < 650)
            ui->Print_Velocidad->setText(" Media");

        else if(velocidad < 1001)
            ui->Print_Velocidad->setText(" Alta");

        else
            ui->Print_Velocidad->setText("Los valores no son correctos");
      //  ui->Print_Velocidad->setText(QString::number(velocidad));

}


