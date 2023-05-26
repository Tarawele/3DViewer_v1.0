#ifndef WIDGET_H
#define WIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QOpenGLWidget>

extern "C" {
#include "../parser/s21_parcer.h"
}

class widget : public QOpenGLWidget {
 public:
  int progection_mode;
  int rib_type;
  char c[100];
  struct result_struct* model;
  widget();
  int rib_width;
  int dot_type;
  int dot_width;

  float back_red;
  float back_green;
  float back_blue;

  float rib_red;
  float rib_green;
  float rib_blue;

  float dot_red;
  float dot_green;
  float dot_blue;

  ~widget();

 protected:
  void paintGL() override;
  void initializeGL() override;
};

#endif  // WIDGET_H
