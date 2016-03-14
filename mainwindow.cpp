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
#include <sstream>

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

    std::ostringstream ostr;

    using namespace fl;

    fl::Engine* engine = new fl::Engine;
    engine->setName("");
    int minTemperatura = ui->minTemp->value();
    int maxTemperatura = ui->maxTemp->value();
    int minHumedad = ui->minHum->value();
    int maxHumedad = ui->maxHum->value();
    int minVelocidad = ui->minVel->value();
    int maxVelocidad = ui->maxVel->value();

  int  minTempMax = minTemperatura + ( minTemperatura * -1) + 10;

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setEnabled(true);
    inputVariable1->setName("temperatura");
    inputVariable1->setRange(minTemperatura, maxTemperatura);
    inputVariable1->addTerm(new fl::Trapezoid("frio", minTemperatura, minTemperatura, 0 , maxTemperatura * 0.2));
    inputVariable1->addTerm(new fl::Triangle("templado", maxTemperatura * 0.1 , maxTemperatura * 0.4 , maxTemperatura * 0.7 ));
    inputVariable1->addTerm(new fl::Trapezoid("caliente", maxTemperatura * 0.6 ,  maxTemperatura * 0.87 , maxTemperatura, maxTemperatura));
    engine->addInputVariable(inputVariable1);

    fl::InputVariable* inputVariable2 = new fl::InputVariable;
    inputVariable2->setEnabled(true);
    inputVariable2->setName("humedad");
    inputVariable2->setRange(0.000, 100.000);
    inputVariable2->addTerm(new fl::Trapezoid("baja", minHumedad, minHumedad, maxHumedad * 0.15, maxHumedad * 0.20));
    inputVariable2->addTerm(new fl::Triangle("media", 20.000, maxHumedad/2, maxHumedad * 0.75));
    inputVariable2->addTerm(new fl::Trapezoid("alta", maxHumedad * 0.7, maxHumedad * 0.80, maxHumedad, maxHumedad));
    engine->addInputVariable(inputVariable2);

    fl::OutputVariable* outputVariable = new fl::OutputVariable;
    outputVariable->setEnabled(true);
    outputVariable->setName("velocidad");
    outputVariable->setRange(minVelocidad , maxVelocidad);
    outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
    outputVariable->setDefuzzifier(new fl::Centroid(200));
    outputVariable->setDefaultValue(fl::nan);
    outputVariable->addTerm(new fl::Trapezoid("baja", 0.000, 0.000, 700.000, 1300.000));
    outputVariable->addTerm(new fl::Triangle("media", 1100.000, 1800.000, 2600.000));
    outputVariable->addTerm(new fl::Trapezoid("alta", 2400.000, 3000.000, 3600.000, 3600.000));
    engine->addOutputVariable(outputVariable);

    fl::RuleBlock* ruleBlock = new fl::RuleBlock;
    ruleBlock->setEnabled(true);
    ruleBlock->setName("");
    ruleBlock->setConjunction(new fl::Minimum);
    ruleBlock->setDisjunction(new fl::Maximum);
    ruleBlock->setActivation(new fl::Minimum);
    ruleBlock->addRule(fl::Rule::parse("if temperatura is frio and humedad is baja then velocidad is baja", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is frio and humedad is media then velocidad is baja", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is frio and humedad is alta then velocidad is media", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is templado and humedad is baja then velocidad is media", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is templado and humedad is media then velocidad is media", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is templado and humedad is alta then velocidad is alta", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is caliente and humedad is baja then velocidad is alta", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is caliente and humedad is media then velocidad is media", engine));
    ruleBlock->addRule(fl::Rule::parse("if temperatura is caliente and humedad is alta then velocidad is alta", engine));
    engine->addRuleBlock(ruleBlock);



       engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid");
       std::string status;
       if (not engine->isReady(&status))
           throw Exception("Engine not ready. "
               "The following errors were encountered:\n" + status, FL_AT);

       //Se almacenan los valores ingresados en la caja de texto
       QString humedad = ui->humedad->text();
       QString temperatura = ui->temperatura->text();

       //Se ingresan los valores a humedad y temperatura
       inputVariable1->setInputValue(temperatura.toFloat());
       inputVariable2->setInputValue(humedad.toFloat());

       //se procesa la informacion
       engine->process();

        //se adquiere la salida despues de fuzzificarse
        float velocidad = outputVariable->getOutputValue();

        ostr << velocidad;

        QString texto = QString::number(velocidad);

        //Comparar la velocidad para mostrarla
        if( velocidad < 1260 )
            ui->Print_Velocidad->setText( "Baja  -> " + texto);

        else if(velocidad < 2300)
            ui->Print_Velocidad->setText(" Media -> " + texto);

        else if(velocidad < 3600)
            ui->Print_Velocidad->setText(" Alta -> " + texto);

        else
            ui->Print_Velocidad->setText("Los valores no son correctos");

}


