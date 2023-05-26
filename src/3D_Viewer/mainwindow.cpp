#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  myWidget = new widget;
  ui->gridLayout_widget->addWidget(myWidget, 0, 0);
  put_settings();
  setWindowTitle("3D_Viewer");
}

MainWindow::~MainWindow() {
  save_settings();
  delete myWidget;
  delete ui;
}

void MainWindow::on_pushButton_clicked() {
  QString filename =
      QFileDialog::getOpenFileName(this, "Choose a file", "/", "*.obj");
  if (QString(filename).isNull()) return;
  ui->filename->setText(filename);

  std::string fname = filename.toStdString();
  strcpy(myWidget->c, fname.c_str());
  if (myWidget->model) res_remove(myWidget->model);
  myWidget->model = get_data(myWidget->c);
  ui->ribs->setText(QString::number(myWidget->model->amount_connections / 2));
  ui->dots->setText(QString::number(myWidget->model->amount_points / 3));
  myWidget->update();
}

// rotate

// rotate x
void MainWindow::on_rotate_x_valueChanged(int value) {
  ui->x_num2->setValue(value);
  if (myWidget->model) {
    affine_rotate(0, (double)value, myWidget->model);
    myWidget->update();
  }
}

void MainWindow::on_x_num2_editingFinished() {
  if (myWidget->model) {
    affine_rotate(0, ui->x_num2->value(), myWidget->model);
    myWidget->update();
  }
}

// rotate y
void MainWindow::on_rotate_y_valueChanged(int value) {
  ui->y_num2->setValue(value);
  if (myWidget->model) {
    affine_rotate(1, (double)value, myWidget->model);
    myWidget->update();
  }
}

void MainWindow::on_y_num2_editingFinished() {
  if (myWidget->model) {
    affine_rotate(1, ui->y_num2->value(), myWidget->model);
    myWidget->update();
  }
}

// rotate z
void MainWindow::on_rotate_z_valueChanged(int value) {
  ui->z_num2->setValue(value);
  if (myWidget->model) {
    affine_rotate(2, (double)value, myWidget->model);
    myWidget->update();
  }
}

void MainWindow::on_z_num2_editingFinished() {
  if (myWidget->model) {
    affine_rotate(2, ui->z_num2->value(), myWidget->model);
    myWidget->update();
  }
}

// move
void MainWindow::on_move_x_editingFinished() {
  if (myWidget->model) {
    affine_shift(0, ui->move_x->value() / 100, myWidget->model);
    myWidget->update();
  }
}

void MainWindow::on_move_y_editingFinished() {
  if (myWidget->model) {
    affine_shift(1, ui->move_y->value() / 100, myWidget->model);
    myWidget->update();
  }
}

void MainWindow::on_move_z_editingFinished() {
  if (myWidget->model) {
    affine_shift(2, ui->move_z->value() / 100, myWidget->model);
    myWidget->update();
  }
}

// scale
void MainWindow::on_scale_valueChanged(int value) {
  if (value > 0)
    ui->scale_num->setText(QString::number(1. + value));
  else
    ui->scale_num->setText(QString::number(1. + value / 10.));
}

void MainWindow::on_scale_sliderReleased() {
  if (myWidget->model) {
    affine_scale(ui->scale_num->text().toDouble(), myWidget->model);
    myWidget->update();
  }
}

// settings

// mode
void MainWindow::on_comboBox_currentIndexChanged(int index) {
  myWidget->progection_mode = index;
  if (myWidget->model) {
    myWidget->update();
  }
}

// ribs
void MainWindow::on_comboBox_2_currentIndexChanged(int index) {
  myWidget->rib_type = index;
  if (myWidget->model) {
    myWidget->update();
  }
}

void MainWindow::on_ribs_thick_valueChanged(int value) {
  myWidget->rib_width = value;
  if (myWidget->model) {
    myWidget->update();
  }
}

// dots
void MainWindow::on_comboBox_3_currentIndexChanged(int index) {
  myWidget->dot_type = index;
  if (myWidget->model) {
    myWidget->update();
  }
}

void MainWindow::on_ribs_thick_2_valueChanged(int value) {
  myWidget->dot_width = value;
  if (myWidget->model) {
    myWidget->update();
  }
}

// background colour
void MainWindow::on_ribs_thick_3_valueChanged(int value) {
  form_color(&myWidget->back_red, &myWidget->back_green, &myWidget->back_blue,
             (float)value);
  myWidget->update();
}

void MainWindow::form_color(float *r, float *g, float *b, float value) {
  if (value == 0) {
    *r = 0;
    *g = 0;
    *b = 0;
  }
  if (value == 1) {
    *r = 0;
    *g = 0;
    *b = 255;
  }
  if (value == 2) {
    *r = 0;
    *g = 255;
    *b = 0;
  }
  if (value == 3) {
    *r = 0;
    *g = 255;
    *b = 255;
  }
  if (value == 4) {
    *r = 255;
    *g = 0;
    *b = 0;
  }
  if (value == 5) {
    *r = 255;
    *g = 0;
    *b = 255;
  }
  if (value == 6) {
    *r = 255;
    *g = 255;
    *b = 0;
  }
  if (value == 7) {
    *r = 255;
    *g = 255;
    *b = 255;
  }
}

// rib color
void MainWindow::on_rib_color_valueChanged(int value) {
  form_color(&myWidget->rib_red, &myWidget->rib_green, &myWidget->rib_blue,
             (float)value);
  myWidget->update();
}

// dot color
void MainWindow::on_dot_color_valueChanged(int value) {
  form_color(&myWidget->dot_red, &myWidget->dot_green, &myWidget->dot_blue,
             (float)value);
  myWidget->update();
}

// setting save
void MainWindow::save_settings() {
  QString name = QString("%1/setting.cfg")
                     .arg(QStandardPaths::writableLocation(
                         QStandardPaths::DesktopLocation));
  QByteArray temp = name.toLocal8Bit();
  char *path_setting = temp.data();
  FILE *file = fopen(path_setting, "w");
  if (file) {
    fprintf(file, "%d %d %d %d %d %d %d %d", myWidget->progection_mode,
            myWidget->rib_type, myWidget->rib_width, myWidget->dot_type,
            myWidget->dot_width, ui->rib_color->value(), ui->dot_color->value(),
            ui->ribs_thick_3->value());
    fclose(file);
  }
}

void MainWindow::put_settings() {
  int rib, back, dot;
  QString name = QString("%1/setting.cfg")
                     .arg(QStandardPaths::writableLocation(
                         QStandardPaths::DesktopLocation));
  QByteArray temp = name.toLocal8Bit();
  char *path_setting = temp.data();
  FILE *file = fopen(path_setting, "r");
  if (file) {
    fscanf(file, "%d %d %d %d %d %d %d %d", &myWidget->progection_mode,
           &myWidget->rib_type, &myWidget->rib_width, &myWidget->dot_type,
           &myWidget->dot_width, &rib, &dot, &back);

    ui->comboBox->setCurrentIndex(myWidget->progection_mode);
    ui->comboBox_2->setCurrentIndex(myWidget->rib_type);
    ui->comboBox_3->setCurrentIndex(myWidget->dot_type);
    ui->ribs_thick->setValue(myWidget->rib_width);
    ui->ribs_thick_2->setValue(myWidget->dot_width);
    ui->ribs_thick_3->setValue(back);
    ui->rib_color->setValue(rib);
    ui->dot_color->setValue(dot);
    fclose(file);
  }
}

void MainWindow::on_screen_pressed() {
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...",
      "Screenshot " +
          QDateTime::currentDateTime().toString("dd.MM.yyyy hh-mm-ss"),
      "BMP (*.bmp);; JPEG (*.jpg *.jpeg)");
  if (file.length()) {
    myWidget->grabFramebuffer().save(file);
  }
}
