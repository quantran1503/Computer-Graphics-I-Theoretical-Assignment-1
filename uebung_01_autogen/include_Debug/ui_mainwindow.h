/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "openglview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    OpenGLView *openGLWidget;
    QWidget *verticalWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *resetViewButton;
    QPushButton *recalcNormalsByAngleButton;
    QPushButton *recalcNormalsByAreaButton;
    QCheckBox *lightMovementCheckBox;
    QLabel *movementExplanationLabel;
    QSpacerItem *verticalSpacer;
    QPushButton *exitButton;
    QStatusBar *statusBar;

    // test
    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 768);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        openGLWidget = new OpenGLView(centralwidget);
        openGLWidget->setObjectName("openGLWidget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        openGLWidget->setBaseSize(QSize(800, 600));

        horizontalLayout->addWidget(openGLWidget);

        verticalWidget = new QWidget(centralwidget);
        verticalWidget->setObjectName("verticalWidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(verticalWidget->sizePolicy().hasHeightForWidth());
        verticalWidget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(verticalWidget);
        verticalLayout->setObjectName("verticalLayout");
        resetViewButton = new QPushButton(verticalWidget);
        resetViewButton->setObjectName("resetViewButton");

        verticalLayout->addWidget(resetViewButton);

        recalcNormalsByAngleButton = new QPushButton(verticalWidget);
        recalcNormalsByAngleButton->setObjectName("recalcNormalsByAngleButton");

        verticalLayout->addWidget(recalcNormalsByAngleButton);

        recalcNormalsByAreaButton = new QPushButton(verticalWidget);
        recalcNormalsByAreaButton->setObjectName("recalcNormalsByAreaButton");

        verticalLayout->addWidget(recalcNormalsByAreaButton);

        lightMovementCheckBox = new QCheckBox(verticalWidget);
        lightMovementCheckBox->setObjectName("lightMovementCheckBox");

        verticalLayout->addWidget(lightMovementCheckBox);

        movementExplanationLabel = new QLabel(verticalWidget);
        movementExplanationLabel->setObjectName("movementExplanationLabel");

        verticalLayout->addWidget(movementExplanationLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        exitButton = new QPushButton(verticalWidget);
        exitButton->setObjectName("exitButton");

        verticalLayout->addWidget(exitButton);


        horizontalLayout->addWidget(verticalWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "TU Darmstadt, GDV1, OpenGL Praxis\303\274bung 1", nullptr));
        resetViewButton->setText(QCoreApplication::translate("MainWindow", "Ansicht zur\303\274cksetzen", nullptr));
        recalcNormalsByAngleButton->setText(QCoreApplication::translate("MainWindow", "Normalen neu berechnen\n"
"(Winkelgewichtung)", nullptr));
        recalcNormalsByAreaButton->setText(QCoreApplication::translate("MainWindow", "Normalen neu berechnen\n"
"(Fl\303\244chengewichtung)", nullptr));
        lightMovementCheckBox->setText(QCoreApplication::translate("MainWindow", "Lichtbewegung", nullptr));
        movementExplanationLabel->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><h4>Bewegungssteuerung:</h4><p><h5>linke Maustaste:</h5><br/>Rotation um Achsen</p><p><h5>rechte Maustaste:</h5><br/>Zoom/Vorw\303\244rts-R\303\274ckw\303\244rts</p><p><h5>mittlere Maustaste:</h5><br/>hoch/runter/links/rechts</p></body></html>", nullptr));
        exitButton->setText(QCoreApplication::translate("MainWindow", "Beenden", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
