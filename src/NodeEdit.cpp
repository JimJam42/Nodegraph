/*
  Copyright (C) 2014 Callum James

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "NodeEdit.h"
#include "ui_NodeEdit.h"

#include <iostream>

NodeEdit::NodeEdit(QWidget *parent) : QDialog(parent), m_ui(new Ui::NodeEdit)
{
    m_ui->setupUi(this);
    connectAndInit();
}

NodeEdit::~NodeEdit()
{
    m_nodeToEdit = NULL;
    delete m_ui;
}

void NodeEdit::keyPressEvent(QKeyEvent *_event)
{
    if (_event->key() == Qt::Key_Return || _event->key() == Qt::Key_Escape || _event->key() == Qt::Key_Enter)
    {
        close();
    }
}

void NodeEdit::setNodeToEdit(GraphNode *_node)
{
    if (_node != NULL)
    {
        m_ui->line_name->setFocus();
        m_nodeToEdit = _node;
        m_ui->line_name->setText(m_nodeToEdit->name().c_str());
        m_ui->line_shortName->setText(m_nodeToEdit->shortName().c_str());
    }
}

void NodeEdit::setNodeName()
{
    if (m_nodeToEdit != NULL)
    {
        m_nodeToEdit->setName(m_ui->line_name->text().toStdString());
        if (m_ui->line_shortName->text() == "")
        {
            m_ui->line_shortName->setText(m_ui->line_name->text().at(0));
        }
    }
}

void NodeEdit::setNodeShortName()
{
    if (m_nodeToEdit != NULL)
    {
        m_nodeToEdit->setShortName(m_ui->line_shortName->text().toStdString());
    }
}

void NodeEdit::connectAndInit()
{
    m_nodeToEdit = NULL;

    m_ui->line_name->setFocus();

    connect(m_ui->line_name,SIGNAL(textChanged(QString)),this,SLOT(setNodeName()));
    connect(m_ui->line_shortName,SIGNAL(textChanged(QString)),this,SLOT(setNodeShortName()));
}
