/********************************************************************************
** Form generated from reading UI file 'NodeEdit.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NODEEDIT_H
#define UI_NODEEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_NodeEdit
{
public:
    QGridLayout *gridLayout;
    QGroupBox *gbx_shortName;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *line_shortName;
    QGroupBox *gbx_longName;
    QVBoxLayout *verticalLayout;
    QLineEdit *line_name;

    void setupUi(QDialog *NodeEdit)
    {
        if (NodeEdit->objectName().isEmpty())
            NodeEdit->setObjectName(QStringLiteral("NodeEdit"));
        NodeEdit->resize(314, 175);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NodeEdit->sizePolicy().hasHeightForWidth());
        NodeEdit->setSizePolicy(sizePolicy);
        NodeEdit->setMinimumSize(QSize(314, 175));
        NodeEdit->setMaximumSize(QSize(314, 175));
        gridLayout = new QGridLayout(NodeEdit);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gbx_shortName = new QGroupBox(NodeEdit);
        gbx_shortName->setObjectName(QStringLiteral("gbx_shortName"));
        verticalLayout_2 = new QVBoxLayout(gbx_shortName);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        line_shortName = new QLineEdit(gbx_shortName);
        line_shortName->setObjectName(QStringLiteral("line_shortName"));
        line_shortName->setMaxLength(4);

        verticalLayout_2->addWidget(line_shortName);


        gridLayout->addWidget(gbx_shortName, 1, 0, 1, 1);

        gbx_longName = new QGroupBox(NodeEdit);
        gbx_longName->setObjectName(QStringLiteral("gbx_longName"));
        verticalLayout = new QVBoxLayout(gbx_longName);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        line_name = new QLineEdit(gbx_longName);
        line_name->setObjectName(QStringLiteral("line_name"));
        line_name->setFocusPolicy(Qt::StrongFocus);
        line_name->setMaxLength(50);

        verticalLayout->addWidget(line_name);


        gridLayout->addWidget(gbx_longName, 0, 0, 1, 1);


        retranslateUi(NodeEdit);

        QMetaObject::connectSlotsByName(NodeEdit);
    } // setupUi

    void retranslateUi(QDialog *NodeEdit)
    {
        NodeEdit->setWindowTitle(QApplication::translate("NodeEdit", "Edit Node", 0));
        gbx_shortName->setTitle(QApplication::translate("NodeEdit", "Short Name", 0));
        line_shortName->setPlaceholderText(QApplication::translate("NodeEdit", "maximum 4 characters", 0));
        gbx_longName->setTitle(QApplication::translate("NodeEdit", "Node Name", 0));
        line_name->setInputMask(QString());
        line_name->setPlaceholderText(QApplication::translate("NodeEdit", "Enter node name", 0));
    } // retranslateUi

};

namespace Ui {
    class NodeEdit: public Ui_NodeEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NODEEDIT_H
