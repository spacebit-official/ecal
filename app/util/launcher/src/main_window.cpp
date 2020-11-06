/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2019 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= eCAL LICENSE =================================
*/

/**
 * @brief eCALLauncher Application
**/

#include "main_window.h"
#include "ui_main_window.h"
#include <ecal/ecal_apps.h>
#include "qpushbutton.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  _ui(new Ui::MainWindow)
{
  this->setWindowFlags(Qt::FramelessWindowHint);
  _ui->setupUi(this);
  _ui->centralWidget->installEventFilter(this);
  connect(_ui->pushButton_minimize, &QPushButton::clicked, [this]() { this->showMinimized();});
  connect(_ui->pushButton_exit_x, &QPushButton::clicked, [this]() { this->close(); });

#ifdef _MSC_VER
  _ecal_dir = qgetenv("ECAL_HOME");
  _ecal_dir += "\\bin\\";
#endif
}

MainWindow::~MainWindow()
{
  delete _ui;
}

void MainWindow::mousePressEvent(QMouseEvent * /*event*/)
{
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == _ui->centralWidget)
  {
    if (event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
      if (mouse_event == nullptr) return false;
      if (mouse_event->button() == Qt::LeftButton)
      {
        _dragPosition = mouse_event->globalPos() - frameGeometry().topLeft();
        return false;
      }
    }
    else if (event->type() == QEvent::MouseMove)
    {
      QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
      if (mouse_event == nullptr) return false;
      if (mouse_event->buttons() & Qt::LeftButton)
      {
        move(mouse_event->globalPos() - _dragPosition);
        return false;
      }
    }
  }
  return false;
}

void MainWindow::on_pushButton_monitor_clicked()
{
  QString pname = eCAL::Apps::MON_GUI;
#ifdef _MSC_VER
  pname += ".exe";
#endif
  QProcess::startDetached("\"" + _ecal_dir + pname + "\"");
}

void MainWindow::on_pushButton_play_clicked()
{
  QString pname = eCAL::Apps::PLAY_GUI;
#ifdef _MSC_VER
  pname += ".exe";
#endif
  QProcess::startDetached("\"" + _ecal_dir + pname + "\"");
}

void MainWindow::on_pushButton_rec_clicked()
{
  QString pname = eCAL::Apps::REC_GUI;
#ifdef _MSC_VER
  pname += ".exe";
#endif
  QProcess::startDetached("\"" + _ecal_dir + pname + "\"");
}

void MainWindow::on_pushButton_about_clicked()
{
  _about_dialog = new AboutDialog(this);
  _about_dialog->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
  _about_dialog->setWindowIcon(QIcon(":about.svg"));
  _about_dialog->exec();
  delete _about_dialog;
}

void MainWindow::on_pushButton_exit_clicked()
{
  this->close();
}