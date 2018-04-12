/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    QGroupBox *varsListGroup;
    QVBoxLayout *verticalLayout;
    QScrollArea *variablesListArea;
    QWidget *scrollWidget;
    QGroupBox *plotGroup;
    QGridLayout *gridLayout_2;
    QSpinBox *nPointsSpinbox;
    QLabel *label;
    QLabel *label_2;
    QtCharts::QChartView *graphicsView;
    QSpinBox *plotDecimSpinbox;
    QPushButton *clearButton;
    QPushButton *pausePlotButton;
    QHBoxLayout *horizontalLayout;
    QCheckBox *logToFileCheckbox;
    QPushButton *setLogLocationButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(648, 649);
        MainWindow->setMinimumSize(QSize(0, 0));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        varsListGroup = new QGroupBox(splitter);
        varsListGroup->setObjectName(QStringLiteral("varsListGroup"));
        verticalLayout = new QVBoxLayout(varsListGroup);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        variablesListArea = new QScrollArea(varsListGroup);
        variablesListArea->setObjectName(QStringLiteral("variablesListArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(variablesListArea->sizePolicy().hasHeightForWidth());
        variablesListArea->setSizePolicy(sizePolicy);
        variablesListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        variablesListArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        variablesListArea->setWidgetResizable(true);
        scrollWidget = new QWidget();
        scrollWidget->setObjectName(QStringLiteral("scrollWidget"));
        scrollWidget->setGeometry(QRect(0, 0, 616, 101));
        variablesListArea->setWidget(scrollWidget);

        verticalLayout->addWidget(variablesListArea);

        splitter->addWidget(varsListGroup);
        plotGroup = new QGroupBox(splitter);
        plotGroup->setObjectName(QStringLiteral("plotGroup"));
        gridLayout_2 = new QGridLayout(plotGroup);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        nPointsSpinbox = new QSpinBox(plotGroup);
        nPointsSpinbox->setObjectName(QStringLiteral("nPointsSpinbox"));
        nPointsSpinbox->setMinimum(10);
        nPointsSpinbox->setMaximum(1000000);
        nPointsSpinbox->setValue(1000);

        gridLayout_2->addWidget(nPointsSpinbox, 1, 1, 1, 1);

        label = new QLabel(plotGroup);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(plotGroup);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        graphicsView = new QtCharts::QChartView(plotGroup);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));

        gridLayout_2->addWidget(graphicsView, 0, 0, 1, 2);

        plotDecimSpinbox = new QSpinBox(plotGroup);
        plotDecimSpinbox->setObjectName(QStringLiteral("plotDecimSpinbox"));
        plotDecimSpinbox->setMinimum(1);
        plotDecimSpinbox->setMaximum(100);
        plotDecimSpinbox->setValue(10);

        gridLayout_2->addWidget(plotDecimSpinbox, 2, 1, 1, 1);

        clearButton = new QPushButton(plotGroup);
        clearButton->setObjectName(QStringLiteral("clearButton"));

        gridLayout_2->addWidget(clearButton, 3, 0, 1, 1);

        pausePlotButton = new QPushButton(plotGroup);
        pausePlotButton->setObjectName(QStringLiteral("pausePlotButton"));
        pausePlotButton->setCheckable(true);
        pausePlotButton->setChecked(false);

        gridLayout_2->addWidget(pausePlotButton, 3, 1, 1, 1);

        splitter->addWidget(plotGroup);

        verticalLayout_2->addWidget(splitter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        logToFileCheckbox = new QCheckBox(centralWidget);
        logToFileCheckbox->setObjectName(QStringLiteral("logToFileCheckbox"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logToFileCheckbox->sizePolicy().hasHeightForWidth());
        logToFileCheckbox->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(logToFileCheckbox);

        setLogLocationButton = new QPushButton(centralWidget);
        setLogLocationButton->setObjectName(QStringLiteral("setLogLocationButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(setLogLocationButton->sizePolicy().hasHeightForWidth());
        setLogLocationButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(setLogLocationButton);


        verticalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "HRI PC Controller", nullptr));
        varsListGroup->setTitle(QApplication::translate("MainWindow", "Variables list", nullptr));
        plotGroup->setTitle(QApplication::translate("MainWindow", "Live graph", nullptr));
        label->setText(QApplication::translate("MainWindow", "Number of points to show:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Decimation:", nullptr));
        clearButton->setText(QApplication::translate("MainWindow", "Clear plot", nullptr));
        pausePlotButton->setText(QApplication::translate("MainWindow", "Pause plot", nullptr));
        logToFileCheckbox->setText(QApplication::translate("MainWindow", "Log to CSV file", nullptr));
        setLogLocationButton->setText(QApplication::translate("MainWindow", "Change location...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
